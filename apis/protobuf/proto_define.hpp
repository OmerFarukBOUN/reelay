#pragma once

#include "proto_node.hpp"
#include <filesystem>

const google::protobuf::FileDescriptor* init_protobuf(
  std::string message_def_str, std::string message_type)
{
  ArrayInputStream raw_input(message_def_str.c_str(), message_def_str.size());
  Tokenizer input(&raw_input, NULL);

  Parser parser;
  if(!parser.Parse(&input, &file_desc_proto)) {
    std::cerr << "Failed to parse .proto definition:" << message_def_str;
    return nullptr;
  }
  if(!file_desc_proto.has_name()) {
    file_desc_proto.set_name(message_type);
  }
  const google::protobuf::FileDescriptor* file_desc =
    pool.BuildFile(file_desc_proto);
  if(file_desc == NULL) {
    std::cerr << "Cannot get file descriptor from file descriptor proto"
              << file_desc_proto.DebugString();
    return nullptr;
  }
  return file_desc;
}

void init_protobufs()
{
  auto pool_ = google::protobuf::DescriptorPool::generated_pool();
  int file_count =
    pool_->FindFileByName("osi_groundtruth.proto")->message_type_count();
  for(int i = 0; i < file_count; ++i) {
    const google::protobuf::Descriptor* descriptor =
      pool_->FindFileByName("osi_groundtruth.proto")->message_type(i);
  }
  const google::protobuf::Descriptor* message_desc =
    pool_->FindMessageTypeByName("osi3.GroundTruth");
  if(message_desc == NULL) {
    std::cerr << "Cannot get message descriptor of message: "
              << "osi3.GroundTruth" << std::endl;
    throw std::runtime_error(
      "Cannot get message descriptor for osi3.GroundTruth");
  }
  if(!std::filesystem::exists("/home/nonroot/protobufs")) {
    std::cerr << "Directory /home/nonroot/protobufs does not exist"
              << std::endl;
    throw std::runtime_error(
      "Directory /home/nonroot/protobufs does not exist");
  }
  for(const auto& entry :
      std::filesystem::directory_iterator("/home/nonroot/protobufs")) {
    if(entry.path().extension() == ".proto") {
      std::ifstream file(entry.path());
      if(file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string name = entry.path().stem().string();
        init_protobuf(content, name);
      }
    }
  }
}