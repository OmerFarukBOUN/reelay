#pragma once
// #include "reelay/monitors.hpp"
// #include "reelay/parser/ptl_inspector.hpp"
// #include "zenoh.hxx"
#include <boost/url.hpp>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <google/protobuf/compiler/parser.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>
#include <stdio.h>

#include <reelay/zenoh_bridge/proto_node/converters.hpp>
#include <reelay/zenoh_bridge/proto_node/path_token.hpp>

using namespace std::chrono_literals;
using google::protobuf::FieldDescriptor;
using google::protobuf::Message;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace google::protobuf::compiler;
using message_type = std::variant<
  std::string,
  int32_t,
  int64_t,
  uint32_t,
  uint64_t,
  float,
  double,
  bool>;
using converter_func = message_type (*)(
  const google::protobuf::Message&,
  const google::protobuf::FieldDescriptor*,
  int);

inline std::unordered_map<std::string, message_type> proto_map;
inline google::protobuf::DynamicMessageFactory factory;
inline google::protobuf::DescriptorPool pool;
inline FileDescriptorProto file_desc_proto;
inline int global_token_no;
inline std::string last_token_no;

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



class proto_node {
 public:
  std::string token_no;
  int array_no;
  const google::protobuf::FieldDescriptor* fieldDesc;
  const google::protobuf::Reflection* reflection;
  const google::protobuf::Message* message;
  const google::protobuf::Descriptor* msgDesc;
  std::unordered_map<path_token, proto_node> children;
  converter_func converter;

  proto_node() : array_no(-1), fieldDesc(nullptr), reflection(nullptr) {}

  message_type convert(const google::protobuf::Message& msg)
  {
    // std::cout << "convert" << std::endl;
    // message_type result = converter(msg, fieldDesc, array_no);
    // std::cout << "convert" << std::endl;
    // return result;
    return converter(msg, fieldDesc, array_no);
  }

  proto_node(const google::protobuf::Message& msg, std::vector<path_token> path)
  {
    std::cout << "constructor" << std::endl;
    message = &msg;
    this->msgDesc = message->GetDescriptor();
    fieldDesc = msgDesc->FindFieldByName(path[0].key);
    if(fieldDesc == nullptr) {
      std::cout << path[0].key << std::endl;
      for(int i = 0; i < msgDesc->field_count(); i++) {
        const FieldDescriptor* field = msgDesc->field(i);
        if(field->name() == path[0].key) {
          fieldDesc = field;
          break;
        }
      }
    }
    if(fieldDesc == nullptr) {
      throw std::runtime_error("Field not found in message");
    }

    reflection = message->GetReflection();
    array_no = path[0].array_no;

    if(path.size() > 1) {
      std::vector<path_token> new_path(path.begin() + 1, path.end());
      if(path[0].array_no != -1) {

        if(children.find(path[1]) == children.end()) {
          std::cout << "array_no != -1" << std::endl;
          children[path[1]] = proto_node(*factory.GetPrototype(fieldDesc->message_type()), new_path);
        }
        else {
          children[path[1]].add(*factory.GetPrototype(fieldDesc->message_type()), new_path);
        }
      }
      else {
        if(children.find(path[1]) == children.end()) {
          children[path[1]] =
            proto_node(*factory.GetPrototype(fieldDesc->message_type()), new_path);
        } else {
          children[path[1]].add(*factory.GetPrototype(fieldDesc->message_type()),
                                 new_path);
        }
      }
    }
    else {
      token_no = std::to_string(global_token_no);
      global_token_no++;
      last_token_no = token_no;
      if(path[0].array_no != -1) {
        switch(fieldDesc->cpp_type()) {
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
      }
      else {
        switch(fieldDesc->cpp_type()) {
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

  void add(const google::protobuf::Message& msg, std::vector<path_token> path)
  {
    std::cout << "add" << std::endl;
    if(path.size() > 1) {
      if(array_no != -1) {
        if (children.find(path[1]) == children.end()) {
          children[path[1]] = proto_node(
            reflection->GetRepeatedMessage(msg, fieldDesc, array_no), path);
        }
        else {
          std::vector<path_token> new_path(path.begin() + 1, path.end());
          children[path[1]].add(
            reflection->GetRepeatedMessage(msg, fieldDesc, array_no), new_path);
        }
      } else {
        if(children.find(path[1]) == children.end()) {
          children[path[1]] =
            proto_node(reflection->GetMessage(msg, fieldDesc), path);
        }
        else {
          std::vector<path_token> new_path(path.begin() + 1, path.end());
          children[path[1]].add(reflection->GetMessage(msg, fieldDesc), new_path);
        }
      }
    }
    else {
      last_token_no = token_no;
    }
  }

  void update(const google::protobuf::Message& msg)
  {
    std::cout << "update" << std::endl;
    message->GetReflection();
    std::cout << "control" << std::endl;
    std::cout << msg.GetTypeName() << std::endl;
    if(children.size() > 0) {
      for(auto& child : children) {
        std::cout << "control2" << std::endl;
        // std::cout << "child:" << child.first.key << " " << child.first.array_no << std::endl;
        if(array_no != -1) {
          child.second.update(
            reflection->GetRepeatedMessage(msg, fieldDesc, array_no));
        }
        else {
          child.second.update(reflection->GetMessage(msg, fieldDesc));
        }
      }
    }
    else {
      std::cout << "control3" << std::endl;
      proto_map[token_no] = convert(msg);
      std::cout << "update bitti" << std::endl;
    }
  }
};



