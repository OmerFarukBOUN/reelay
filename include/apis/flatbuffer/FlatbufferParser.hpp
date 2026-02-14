#pragma once

#include <pub_sub_connection/globals.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <stdexcept>
#include <memory>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/reflection.h>
#include <flatbuffers/idl.h>

namespace fs = std::filesystem;

// Custom deleter type for non-owning pointers
struct NoOpDeleter {
    void operator()(reflection::Schema*) const {}
};

// Initialize flatbuffers schemas from folder
inline std::unordered_map<std::string, std::unique_ptr<reflection::Schema, NoOpDeleter>> global_schema_map;
inline std::unordered_map<std::string, std::vector<uint8_t>> global_schema_buffers;

void init_flatbuffers(const std::string& folder_path) {
    global_schema_map.clear();
    global_schema_buffers.clear();
    
    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::string ext = entry.path().extension().string();
            
            if (ext == ".fbs" || ext == ".bfbs") {
                std::ifstream file(path, std::ios::binary);
                if (!file) {
                    throw std::runtime_error("Cannot open file: " + path);
                }
                
                std::string schema_name = entry.path().stem().string();
                
                if (ext == ".bfbs") {
                    // Binary schema file
                    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
                                               std::istreambuf_iterator<char>());
                    
                    flatbuffers::Verifier verifier(buffer.data(), buffer.size());
                    if (!reflection::VerifySchemaBuffer(verifier)) {
                        throw std::runtime_error("Invalid schema file: " + path);
                    }
                    
                    global_schema_buffers[schema_name] = std::move(buffer);
                    const reflection::Schema* schema = 
                        reflection::GetSchema(global_schema_buffers[schema_name].data());
                    
                    global_schema_map[schema_name] = 
                        std::unique_ptr<reflection::Schema, NoOpDeleter>(
                            const_cast<reflection::Schema*>(schema),
                            NoOpDeleter{}
                        );
                } else {
                    // .fbs file - parse and compile
                    std::string schema_content((std::istreambuf_iterator<char>(file)),
                                              std::istreambuf_iterator<char>());
                    
                    flatbuffers::Parser parser;
                    if (!parser.Parse(schema_content.c_str())) {
                        throw std::runtime_error("Failed to parse schema: " + path + 
                                               "\nError: " + parser.error_);
                    }
                    
                    parser.Serialize();
                    global_schema_buffers[schema_name] = std::vector<uint8_t>(
                        parser.builder_.GetBufferPointer(),
                        parser.builder_.GetBufferPointer() + parser.builder_.GetSize()
                    );
                    
                    const reflection::Schema* schema = 
                        reflection::GetSchema(global_schema_buffers[schema_name].data());
                    
                    global_schema_map[schema_name] = 
                        std::unique_ptr<reflection::Schema, NoOpDeleter>(
                            const_cast<reflection::Schema*>(schema),
                            NoOpDeleter{}
                        );
                }
            }
        }
    }
}

// Structure to hold pre-computed access information
struct FieldAccessor {
    std::vector<flatbuffers::voffset_t> offsets;  // Pre-computed offsets for nested access
    std::vector<int> array_indices;                // Array indices for vector fields
    std::vector<uint32_t> object_indices;          // Schema object indices for nested objects
    reflection::BaseType final_type;               // Type of the final field
    int64_t default_integer;                       // Default value for integers
    double default_real;                           // Default value for floats/doubles
};

// Zero-copy Flatbuffer parser with pre-computed accessors
template<typename InMsg = std::vector<uint8_t>, 
         typename MonitorMsg = std::unordered_map<std::string, message_type>>
class FlatbufferParser : public ReelayParser<InMsg, MonitorMsg> {
public:
    FlatbufferParser(const std::string& schema_name)
        : schema_name_(schema_name) {
        
        auto it = global_schema_map.find(schema_name);
        if (it == global_schema_map.end()) {
            throw std::runtime_error("Schema not found: " + schema_name);
        }
        schema_ = it->second.get();
        
        // Pre-compute all field accessors
        precompute_accessors();
    }
    
    MonitorMsg parse(const InMsg& in) override {
        // Zero-copy: work directly with input buffer
        const uint8_t* buffer = in.data();
        size_t size = in.size();
        
        // Verify buffer
        flatbuffers::Verifier verifier(buffer, size);
        if (!verifier.VerifyBuffer<flatbuffers::Table>(nullptr)) {
            throw std::runtime_error("Invalid flatbuffer data");
        }
        
        // Get root table
        const flatbuffers::Table* root = flatbuffers::GetAnyRoot(buffer);
        
        // Extract values using pre-computed accessors (no loops during extraction!)
        for (const auto& [token, accessor] : token_to_accessor_) {
            message_type value = extract_value_direct(root, accessor, buffer);
            global_token2value_map[token] = value;
        }
        
        return global_token2value_map;
    }
    
private:
    std::string schema_name_;
    const reflection::Schema* schema_;
    std::unordered_map<std::string, FieldAccessor> token_to_accessor_;
    
    void precompute_accessors() {
        const reflection::Object* root_obj = schema_->root_table();
        
        // Build accessor for each path in path2token_map
        for (const auto& [path, token] : this->path2token_map) {
            FieldAccessor accessor = build_accessor(root_obj, path);
            token_to_accessor_[token] = accessor;
        }
    }
    
    FieldAccessor build_accessor(const reflection::Object* root_obj,
                                 const std::vector<path_token>& path) {
        FieldAccessor accessor;
        const reflection::Object* current_obj = root_obj;
        
        for (size_t i = 0; i < path.size(); ++i) {
            const path_token& token = path[i];
            const auto* fields = current_obj->fields();
            const reflection::Field* field = nullptr;
            
            // Find field by name
            for (const auto* f : *fields) {
                if (f->name()->str() == token.key) {
                    field = f;
                    break;
                }
            }
            
            if (!field) {
                throw std::runtime_error("Field not found: " + token.key);
            }
            
            // Store offset
            accessor.offsets.push_back(field->offset());
            
            // Handle vector fields
            if (token.array_no >= 0) {
                accessor.array_indices.push_back(token.array_no);
                
                if (field->type()->base_type() == reflection::Obj) {
                    uint32_t obj_idx = field->type()->index();
                    accessor.object_indices.push_back(obj_idx);
                    current_obj = schema_->objects()->Get(obj_idx);
                } else {
                    accessor.object_indices.push_back(0xFFFFFFFF); // Marker for non-object
                }
            } else {
                accessor.array_indices.push_back(-1);
                
                if (field->type()->base_type() == reflection::Obj) {
                    uint32_t obj_idx = field->type()->index();
                    accessor.object_indices.push_back(obj_idx);
                    current_obj = schema_->objects()->Get(obj_idx);
                } else {
                    accessor.object_indices.push_back(0xFFFFFFFF);
                }
            }
            
            // If this is the last field, store type and defaults
            if (i == path.size() - 1) {
                accessor.final_type = field->type()->base_type();
                accessor.default_integer = field->default_integer();
                accessor.default_real = field->default_real();
            }
        }
        
        return accessor;
    }
    
    // Direct extraction using pre-computed accessor - NO LOOPS!
    message_type extract_value_direct(const flatbuffers::Table* root,
                                      const FieldAccessor& accessor,
                                      const uint8_t* root_buffer) {
        const flatbuffers::Table* current_table = root;
        
        // Navigate to the target field using pre-computed offsets
        size_t depth = accessor.offsets.size();
        
        for (size_t i = 0; i < depth - 1; ++i) {
            flatbuffers::voffset_t offset = accessor.offsets[i];
            int array_idx = accessor.array_indices[i];
            
            if (array_idx >= 0) {
                // Vector field
                auto vec_ptr = current_table->GetPointer<const uint8_t*>(offset);
                if (!vec_ptr) {
                    throw std::runtime_error("Vector field is null");
                }
                
                auto vec = reinterpret_cast<const flatbuffers::Vector<flatbuffers::Offset<void>>*>(vec_ptr);
                if (array_idx >= static_cast<int>(vec->size())) {
                    throw std::runtime_error("Array index out of bounds");
                }
                
                current_table = reinterpret_cast<const flatbuffers::Table*>(vec->Get(array_idx));
            } else {
                // Regular nested object
                current_table = current_table->GetPointer<const flatbuffers::Table*>(offset);
                if (!current_table) {
                    throw std::runtime_error("Nested object is null");
                }
            }
        }
        
        // Extract final value
        flatbuffers::voffset_t final_offset = accessor.offsets[depth - 1];
        int final_array_idx = accessor.array_indices[depth - 1];
        
        // Handle final field being in a vector
        if (final_array_idx >= 0) {
            auto vec_ptr = current_table->GetPointer<const uint8_t*>(final_offset);
            if (!vec_ptr) {
                throw std::runtime_error("Vector field is null");
            }
            
            switch (accessor.final_type) {
                case reflection::Int: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<int32_t>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::Long: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<int64_t>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::UInt: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<uint32_t>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::ULong: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<uint64_t>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::Float: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<float>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::Double: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<double>*>(vec_ptr);
                    return vec->Get(final_array_idx);
                }
                case reflection::Bool: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<uint8_t>*>(vec_ptr);
                    return vec->Get(final_array_idx) != 0;
                }
                case reflection::String: {
                    auto vec = reinterpret_cast<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>*>(vec_ptr);
                    auto str = vec->Get(final_array_idx);
                    return str ? std::string(str->c_str(), str->size()) : std::string();
                }
                default:
                    throw std::runtime_error("Unsupported vector element type");
            }
        }
        
        // Direct field access
        switch (accessor.final_type) {
            case reflection::String: {
                auto str_offset = current_table->GetPointer<flatbuffers::uoffset_t>(final_offset);
                if (str_offset) {
                    auto str = reinterpret_cast<const flatbuffers::String*>(str_offset);
                    return std::string(str->c_str(), str->size());
                }
                return std::string();
            }
            case reflection::Int:
                return current_table->GetField<int32_t>(final_offset, 
                    static_cast<int32_t>(accessor.default_integer));
            case reflection::Long:
                return current_table->GetField<int64_t>(final_offset, 
                    accessor.default_integer);
            case reflection::UInt:
                return current_table->GetField<uint32_t>(final_offset, 
                    static_cast<uint32_t>(accessor.default_integer));
            case reflection::ULong:
                return current_table->GetField<uint64_t>(final_offset, 
                    static_cast<uint64_t>(accessor.default_integer));
            case reflection::Float:
                return current_table->GetField<float>(final_offset, 
                    static_cast<float>(accessor.default_real));
            case reflection::Double:
                return current_table->GetField<double>(final_offset, 
                    accessor.default_real);
            case reflection::Bool:
                return current_table->GetField<uint8_t>(final_offset, 
                    static_cast<uint8_t>(accessor.default_integer)) != 0;
            default:
                throw std::runtime_error("Unsupported field type");
        }
    }
};