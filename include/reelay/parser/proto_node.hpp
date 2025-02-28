#include "reelay/monitors.hpp"
#include "reelay/parser/ptl_inspector.hpp"
#include "zenoh.hxx"
#include <boost/url.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <unordered_map>
#include <vector>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>

#include <google/protobuf/compiler/parser.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>


using namespace std::chrono_literals;
using google::protobuf::Message;
using google::protobuf::FieldDescriptor;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace google::protobuf::compiler;

using time_type = int64_t;
using input_type = reelay::json;
using output_type = reelay::json;
using config_type = reelay::json;
using message_type =std::variant<std::string, int32_t, int64_t, uint32_t, uint64_t, float, double, bool>;
// using converter_type = message_type()(google::protobuf::Message);
using converter_type = std::function<message_type(const google::protobuf::Message&)>;


std::unordered_map<std::string, message_type> proto_map;

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

int global_token_no = 0;
std::string last_token_no = "";

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
      return std::hash<std::string>()(obj.key) ^ std::hash<int>()(obj.array_no);
    }
  };
}

class proto_node {
  public:
    std::string token_no;
    int array_no;
    const google::protobuf::FieldDescriptor* fieldDesc;
    const google::protobuf::Reflection* reflection;
    std::unordered_map<object, proto_node> children;
    converter_type converter;

    proto_node(const google::protobuf::Message& msg, std::vector<object> path) {
      fieldDesc = msg.GetDescriptor()->FindFieldByName(path[0].key);
      reflection = msg.GetReflection();
      array_no = path[0].array_no;

      if (path.size() > 1) {
        std::vector<object> new_path(path.begin() + 1, path.end());
        if (path[0].array_no != -1) {
          if (children.find(path[1]) == children.end()) {
            children[path[1]] = proto_node(reflection->GetRepeatedMessage(msg, fieldDesc, array_no), new_path);
          } else {
            children[path[1]].add(reflection->GetRepeatedMessage(msg, fieldDesc, array_no), new_path);
          }
        } else {
          children[path[1]] = proto_node(reflection->GetMessage(msg, fieldDesc), new_path);
        }
      } else {
        token_no = std::to_string(global_token_no);
        global_token_no++;
        last_token_no = token_no;
        if (path[0].array_no != -1) {
          switch (fieldDesc->cpp_type())
          {
          case FieldDescriptor::CPPTYPE_STRING:
            converter = std::bind(&proto_node::convert_to_repeated_string, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_INT32:
            converter = std::bind(&proto_node::convert_to_repeated_int32, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_INT64:
            converter = std::bind(&proto_node::convert_to_repeated_int64, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_UINT32:
            converter = std::bind(&proto_node::convert_to_repeated_uint32, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_UINT64:
            converter = std::bind(&proto_node::convert_to_repeated_uint64, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_FLOAT:
            converter = std::bind(&proto_node::convert_to_repeated_float, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_DOUBLE:
            converter = std::bind(&proto_node::convert_to_repeated_double, this, std::placeholders::_1);
            break;
          case FieldDescriptor::CPPTYPE_BOOL:
            converter = std::bind(&proto_node::convert_to_repeated_bool, this, std::placeholders::_1);
            break;
          default:
            break;
          }
        } else {
          switch (fieldDesc->cpp_type())
        {
        case FieldDescriptor::CPPTYPE_STRING:
          converter = std::bind(&proto_node::convert_to_string, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_INT32:
          converter = std::bind(&proto_node::convert_to_int32, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_INT64:
          converter = std::bind(&proto_node::convert_to_int64, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_UINT32:
          converter = std::bind(&proto_node::convert_to_uint32, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_UINT64:
          converter = std::bind(&proto_node::convert_to_uint64, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_FLOAT:
          converter = std::bind(&proto_node::convert_to_float, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_DOUBLE:
          converter = std::bind(&proto_node::convert_to_double, this, std::placeholders::_1);
          break;
        case FieldDescriptor::CPPTYPE_BOOL:
          converter = std::bind(&proto_node::convert_to_bool, this, std::placeholders::_1);
          break;
        default:
          break;
        }
        }
      }
    }

    void add(const google::protobuf::Message& msg, std::vector<object> path) {
      if (path.size() > 1) {
        if (array_no!=-1) {

        }
        if (children.find(path[1]) == children.end()) {
          children[path[1]] = proto_node(reflection->GetMessage(msg, fieldDesc), path);
        } else {
          std::vector<object> new_path(path.begin() + 1, path.end());
          children[path[1]].add(reflection->GetMessage(msg, fieldDesc), new_path);
        }
      } else {
        last_token_no = token_no;
      }
    }

    message_type convert_to_string(const google::protobuf::Message& msg) {
      return reflection->GetString(msg, fieldDesc);
    }

    message_type convert_to_int32(const google::protobuf::Message& msg) {
      return reflection->GetInt32(msg, fieldDesc);
    }

    message_type convert_to_int64(const google::protobuf::Message& msg) {
      return reflection->GetInt64(msg, fieldDesc);
    }

    message_type convert_to_uint32(const google::protobuf::Message& msg) {
      return reflection->GetUInt32(msg, fieldDesc);
    }

    message_type convert_to_uint64(const google::protobuf::Message& msg) {
      return reflection->GetUInt64(msg, fieldDesc);
    }

    message_type convert_to_float(const google::protobuf::Message& msg) {
      return reflection->GetFloat(msg, fieldDesc);
    }

    message_type convert_to_double(const google::protobuf::Message& msg) {
      return reflection->GetDouble(msg, fieldDesc);
    }

    message_type convert_to_bool(const google::protobuf::Message& msg) {
      return reflection->GetBool(msg, fieldDesc);
    }

    message_type convert_to_repeated_string(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedString(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_int32(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedInt32(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_int64(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedInt64(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_uint32(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedUInt32(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_uint64(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedUInt64(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_float(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedFloat(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_double(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedDouble(msg, fieldDesc, array_no);
    }

    message_type convert_to_repeated_bool(const google::protobuf::Message& msg) {
      return reflection->GetRepeatedBool(msg, fieldDesc, array_no);
    }

    void update(const google::protobuf::Message& msg) {
      if (children.size() > 0) {
        for (auto& child : children) {
          if (child.second.array_no != -1) {
            child.second.update(reflection->GetRepeatedMessage(msg, fieldDesc, array_no));
          } else {
            child.second.update(reflection->GetMessage(msg, fieldDesc));
          }
        }
      } else {
        proto_map[token_no] = converter(msg);
      }
    }

};

class proto_mapper {
  public:
    std::unordered_map<object, proto_node> children;
    google::protobuf::Message* mutable_msg;

    proto_mapper(std::string message_def_str, std::string message_type) {
      ArrayInputStream raw_input(message_def_str.c_str(), message_def_str.size());
      Tokenizer input(&raw_input, NULL);
      FileDescriptorProto file_desc_proto;
      Parser parser;
      if (!parser.Parse(&input, &file_desc_proto)) {
        std::cerr << "Failed to parse .proto definition:" << message_def_str;
        return;
      }
      if (!file_desc_proto.has_name()) {
        file_desc_proto.set_name(message_type);
      }
      google::protobuf::DescriptorPool pool;
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
        std::cerr << "Cannot get message descriptor of message: " << message_type
          << ", DebugString(): " << file_desc->DebugString();
        return;
      }
      google::protobuf::DynamicMessageFactory factory;
      const google::protobuf::Message* prototype_msg =
        factory.GetPrototype(message_desc); // prototype_msg is immutable
      if (prototype_msg == NULL) {
        std::cerr << "Cannot create prototype message from message descriptor";
        return;
      }
      mutable_msg = prototype_msg->New();
      if (mutable_msg == NULL) {
        std::cerr << "Failed in prototype_msg->New(); to create mutable message";
        return;
      }
    }

    void add(const google::protobuf::Message& msg, std::vector<object> path) {
      if (children.find(path[0]) == children.end()) {
        children[path[0]] = proto_node(*mutable_msg, path);
      } else {
        std::vector<object> new_path(path.begin() + 1, path.end());
        children[path[0]].add(*mutable_msg, new_path);
      }
    }

    void update(const std::vector<uint8_t>& msg) {
      mutable_msg->ParseFromArray(msg.data(), msg.size());
      for (auto& child : children) {
        child.second.update(*mutable_msg);
      }
    }

};

