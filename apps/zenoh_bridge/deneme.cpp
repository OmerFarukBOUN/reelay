
// #include "reelay/monitors.hpp"
// #include "reelay/parser/ptl_inspector.hpp"
// #include "zenoh.hxx"
#include <google/protobuf/compiler/parser.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>
#include <stdio.h>

#include <boost/url.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std::chrono_literals;
using google::protobuf::FieldDescriptor;
using google::protobuf::Message;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace google::protobuf::compiler;
using message_type = std::variant<std::string, int32_t, int64_t, uint32_t,
                                  uint64_t, float, double, bool>;
using converter_func =
    message_type (*)(const google::protobuf::Message&,
                     const google::protobuf::FieldDescriptor*, int);

std::unordered_map<std::string, message_type> proto_map;

static message_type convert_uint32(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetUInt32(msg, field);
}

static message_type convert_uint64(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetUInt64(msg, field);
}

static message_type convert_int32(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetInt32(msg, field);
}

static message_type convert_int64(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetInt64(msg, field);
}

static message_type convert_float(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetFloat(msg, field);
}

static message_type convert_double(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetDouble(msg, field);
}

static message_type convert_bool(const google::protobuf::Message& msg,
                                 const google::protobuf::FieldDescriptor* field,
                                 int array_no) {
    return msg.GetReflection()->GetBool(msg, field);
}

static message_type convert_string(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetString(msg, field);
}

static message_type convert_repeated_uint32(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedUInt32(msg, field, array_no);
}

static message_type convert_repeated_uint64(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedUInt64(msg, field, array_no);
}

static message_type convert_repeated_int32(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedInt32(msg, field, array_no);
}

static message_type convert_repeated_int64(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedInt64(msg, field, array_no);
}

static message_type convert_repeated_float(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedFloat(msg, field, array_no);
}

static message_type convert_repeated_double(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedDouble(msg, field, array_no);
}

static message_type convert_repeated_bool(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedBool(msg, field, array_no);
}

static message_type convert_repeated_string(
    const google::protobuf::Message& msg,
    const google::protobuf::FieldDescriptor* field, int array_no) {
    return msg.GetReflection()->GetRepeatedString(msg, field, array_no);
}

enum class message_type_enum {
    STRING,
    INT32,
    INT64,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE,
    BOOL
};

typedef struct object {
    std::string key;
    int array_no;

    bool operator==(const object& other) const {
        return key == other.key && array_no == other.array_no;
    }
} object;

namespace std {
template <>
struct hash<object> {
    std::size_t operator()(const object& obj) const {
        return std::hash<std::string>()(obj.key) ^
               std::hash<int>()(obj.array_no);
    }
};
}  // namespace std

class proto_node {
   public:
    static int global_token_no;
    static std::string last_token_no;
    std::string token_no;
    int array_no;
    const google::protobuf::FieldDescriptor* fieldDesc;
    const google::protobuf::Reflection* reflection;
    const google::protobuf::Message* message;
    const google::protobuf::Descriptor* msgDesc;
    std::unordered_map<object, proto_node> children;
    converter_func converter;

    proto_node() : array_no(-1), fieldDesc(nullptr), reflection(nullptr) {}

    message_type convert(const google::protobuf::Message& msg) {
        return converter(msg, fieldDesc, array_no);
    }

    proto_node(const google::protobuf::Message& msg, std::vector<object> path) {
        message = &msg;
        this->msgDesc = message->GetDescriptor();
        fieldDesc = msgDesc->FindFieldByName(path[0].key);
        if (fieldDesc == nullptr) {
            std::cout << path[0].key << std::endl;
            for (int i = 0; i < msgDesc->field_count(); i++) {
                const FieldDescriptor* field = msgDesc->field(i);
                if (field->name() == path[0].key) {
                    fieldDesc = field;
                    break;
                }
            }
        }
        if (fieldDesc == nullptr) {
            throw std::runtime_error("Field not found in message");
        }
        reflection = message->GetReflection();
        array_no = path[0].array_no;

        if (path.size() > 1) {
            std::vector<object> new_path(path.begin() + 1, path.end());
            if (path[0].array_no != -1) {
                if (children.find(path[1]) == children.end()) {
                    children[path[1]] =
                        proto_node(reflection->GetRepeatedMessage(
                                       *message, fieldDesc, array_no),
                                   new_path);
                } else {
                    children[path[1]].add(reflection->GetRepeatedMessage(
                                              *message, fieldDesc, array_no),
                                          new_path);
                }
            } else {
                children[path[1]] = proto_node(
                    reflection->GetMessage(*message, fieldDesc), new_path);
            }
        } else {
            token_no = std::to_string(global_token_no);
            global_token_no++;
            last_token_no = token_no;
            if (path[0].array_no != -1) {
                switch (fieldDesc->cpp_type()) {
                    case FieldDescriptor::CPPTYPE_STRING:
                        converter = &convert_repeated_string;
                        break;
                    case FieldDescriptor::CPPTYPE_INT32:
                        converter = &convert_repeated_int32;
                        break;
                    case FieldDescriptor::CPPTYPE_INT64:
                        converter = &convert_repeated_int64;
                        break;
                    case FieldDescriptor::CPPTYPE_UINT32:
                        converter = &convert_repeated_uint32;
                        break;
                    case FieldDescriptor::CPPTYPE_UINT64:
                        converter = &convert_repeated_uint64;
                        break;
                    case FieldDescriptor::CPPTYPE_FLOAT:
                        converter = &convert_repeated_float;
                        break;
                    case FieldDescriptor::CPPTYPE_DOUBLE:
                        converter = &convert_repeated_double;
                        break;
                    case FieldDescriptor::CPPTYPE_BOOL:
                        converter = &convert_repeated_bool;
                        break;
                    default:
                        break;
                }
            } else {
                switch (fieldDesc->cpp_type()) {
                    case FieldDescriptor::CPPTYPE_STRING:
                        converter = &convert_string;
                        break;
                    case FieldDescriptor::CPPTYPE_INT32:
                        converter = &convert_int32;
                        break;
                    case FieldDescriptor::CPPTYPE_INT64:
                        converter = &convert_int64;
                        break;
                    case FieldDescriptor::CPPTYPE_UINT32:
                        converter = &convert_uint32;
                        break;
                    case FieldDescriptor::CPPTYPE_UINT64:
                        converter = &convert_uint64;
                        break;
                    case FieldDescriptor::CPPTYPE_FLOAT:
                        converter = &convert_float;
                        break;
                    case FieldDescriptor::CPPTYPE_DOUBLE:
                        converter = &convert_double;
                        break;
                    case FieldDescriptor::CPPTYPE_BOOL:
                        converter = &convert_bool;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void add(const google::protobuf::Message& msg, std::vector<object> path) {
        if (path.size() > 1) {
            if (array_no != -1) {
            }
            if (children.find(path[1]) == children.end()) {
                children[path[1]] =
                    proto_node(reflection->GetMessage(msg, fieldDesc), path);
            } else {
                std::vector<object> new_path(path.begin() + 1, path.end());
                children[path[1]].add(reflection->GetMessage(msg, fieldDesc),
                                      new_path);
            }
        } else {
            last_token_no = token_no;
        }
    }

    void update(const google::protobuf::Message& msg) {
        std::cout << "update" << std::endl;
        message->GetReflection();
        std::cout << "control" << std::endl;
        if (children.size() > 0) {
            for (auto& child : children) {
                if (child.second.array_no != -1) {
                    child.second.update(reflection->GetRepeatedMessage(
                        msg, fieldDesc, array_no));
                } else {
                    child.second.update(reflection->GetMessage(msg, fieldDesc));
                }
            }
        } else {
            std::cout << msg.GetTypeName() << std::endl;
            proto_map[token_no] = convert(msg);
        }
    }
};

int proto_node::global_token_no = 0;
std::string proto_node::last_token_no = "";

class proto_mapper {
   public:
    std::unordered_map<object, proto_node> children;
    const google::protobuf::Message* prototype_msg;
    google::protobuf::DynamicMessageFactory factory;
    google::protobuf::DescriptorPool pool;
    FileDescriptorProto file_desc_proto;

    proto_mapper() : prototype_msg(nullptr) {}

    proto_mapper(std::string message_def_str, std::string message_type) {
        ArrayInputStream raw_input(message_def_str.c_str(),
                                   message_def_str.size());
        Tokenizer input(&raw_input, NULL);

        Parser parser;
        if (!parser.Parse(&input, &file_desc_proto)) {
            std::cerr << "Failed to parse .proto definition:"
                      << message_def_str;
            return;
        }
        if (!file_desc_proto.has_name()) {
            file_desc_proto.set_name(message_type);
        }
        const google::protobuf::FileDescriptor* file_desc =
            pool.BuildFile(file_desc_proto);
        if (file_desc == NULL) {
            std::cerr << "Cannot get file descriptor from file descriptor proto"
                      << file_desc_proto.DebugString();
            return;
        }
        const google::protobuf::Descriptor* message_desc =
            file_desc->FindMessageTypeByName(message_type);
        if (message_desc == NULL) {
            std::cerr << "Cannot get message descriptor of message: "
                      << message_type
                      << ", DebugString(): " << file_desc->DebugString();
            return;
        }
        prototype_msg =
            factory.GetPrototype(message_desc);  // prototype_msg is immutable
        if (prototype_msg == NULL) {
            std::cerr
                << "Cannot create prototype message from message descriptor";
            return;
        }
    }

    void add(std::vector<object> path) {
        if (children.find(path[0]) == children.end()) {
            children[path[0]] = proto_node(*prototype_msg, path);
        } else {
            std::vector<object> new_path(path.begin() + 1, path.end());
            children[path[0]].add(*prototype_msg, new_path);
        }
    }

    void update(const std::vector<uint8_t>& msg) {
        google::protobuf::Message* new_msg = prototype_msg->New();
        prototype_msg->GetDescriptor();
        children.at({"AppLedStateOn", -1}).message->GetDescriptor();
        std::cout << "control" << std::endl;
        if (!new_msg->ParseFromArray(msg.data(), msg.size())) {
            std::cerr << "Failed to parse value in buffer";
        }
        children.at({"AppLedStateOn", -1}).message->GetReflection();
        // std::cout << mutable_msg->DebugString() << std::endl;
        std::cout << new_msg->DebugString() << std::endl;
        for (auto& child : children) {
            child.second.update(*new_msg);
        }
    }
};

proto_mapper global_proto_mapper;

int main() {
    char text[] =
        "syntax = \"proto2\";\n"
        "message APIPort3 {"
        "required uint32 AppLedStateOn = 1;"
        "required uint32 PotiPercentage = 2;"
        "required uint32 VDD = 3;"
        "}";
    std::string message_type("APIPort3");
    std::cout << "first" << std::endl;
    auto mapper = proto_mapper(text, message_type);
    std::cout << "second" << std::endl;
    std::vector<object> path1 = {{"AppLedStateOn", -1}};
    std::cout << "third" << std::endl;
    std::vector<object> path2 = {{"PotiPercentage", -1}};
    std::cout << "fourth" << std::endl;
    std::vector<object> path3 = {{"VDD", -1}};
    std::cout << "fifth" << std::endl;
    mapper.add(path1);
    mapper.children.at(path1[0]).message->GetDescriptor();
    std::cout << "sixth" << std::endl;
    mapper.add(path2);
    std::cout << "seventh" << std::endl;
    mapper.add(path3);
    std::cout << "eighth" << std::endl;
    mapper.prototype_msg->GetDescriptor();
    std::vector<uint8_t> buffer = {0x08, 0x02, 0x10, 0x64, 0x18, 0xF5, 0x2D};
    std::vector<uint8_t> buffer2 = {0x08, 0x00, 0x10, 0x64, 0x18, 0xF5, 0x2D};
    std::cout << "ninth" << std::endl;
    mapper.update(buffer);
    mapper.update(buffer2);
    std::cout << "tenth" << std::endl;
    for (auto& [key, value] : proto_map) {
        std::cout << key << " -> ";
        if (std::holds_alternative<std::string>(value)) {
            std::cout << std::get<std::string>(value) << std::endl;
        } else if (std::holds_alternative<int32_t>(value)) {
            std::cout << std::get<int32_t>(value) << std::endl;
        } else if (std::holds_alternative<int64_t>(value)) {
            std::cout << std::get<int64_t>(value) << std::endl;
        } else if (std::holds_alternative<uint32_t>(value)) {
            std::cout << std::get<uint32_t>(value) << std::endl;
        } else if (std::holds_alternative<uint64_t>(value)) {
            std::cout << std::get<uint64_t>(value) << std::endl;
        } else if (std::holds_alternative<float>(value)) {
            std::cout << std::get<float>(value) << std::endl;
        } else if (std::holds_alternative<double>(value)) {
            std::cout << std::get<double>(value) << std::endl;
        } else if (std::holds_alternative<bool>(value)) {
            std::cout << std::get<bool>(value) << std::endl;
        }
    }
    return 0;
}
