#pragma once

#include "proto_define.hpp"
#include "proto_node.hpp"
#include <reelay/pub_sub_connection/globals.hpp>

class proto_mapper {
 public:
  std::unordered_map<path_token, proto_node> children;
  const google::protobuf::Message* prototype_msg;

  proto_mapper() : prototype_msg(nullptr) {}

  proto_mapper(std::string message_type)
  {
    const google::protobuf::Descriptor* message_desc =
      pool.FindMessageTypeByName(message_type);
    if(message_desc == NULL) {
      message_desc = google::protobuf::DescriptorPool::generated_pool()
                       ->FindMessageTypeByName(message_type);
      if(message_desc == NULL) {
        std::cerr << "Cannot get message descriptor of message: "
                  << message_type << std::endl;
        return;
      }
    }
    prototype_msg =
      factory.GetPrototype(message_desc);  // prototype_msg is immutable
    if(prototype_msg == NULL) {
      std::cerr << "Cannot create prototype message from message descriptor";
      return;
    }
    for (const auto& [path, token] : global_path2token_map) {
      token_to_add = token;
      this->add(path);
    }
  }

  void add(std::vector<path_token> path)
  {
    // std::cout << "Adding: proto_mapper: " << std::endl;
    if(children.find(path[0]) == children.end()) {
      children[path[0]] = proto_node(*prototype_msg, path);
    }
    else {
      std::vector<path_token> new_path(path.begin() + 1, path.end());
      children[path[0]].add(*prototype_msg, new_path);
    }
  }

  void update(const std::vector<uint8_t>& msg)
  {
    google::protobuf::Message* new_msg = prototype_msg->New();
    if(!new_msg->ParseFromArray(msg.data(), msg.size())) {
      std::cerr << "Failed to parse value in buffer";
      exit(EXIT_FAILURE);
    }
    for(auto& child : children) {
      child.second.update(*new_msg);
    }
  }
};
