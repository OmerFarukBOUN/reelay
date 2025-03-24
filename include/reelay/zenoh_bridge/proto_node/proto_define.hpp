#pragma once

#include <reelay/zenoh_bridge/proto_node/proto_node.hpp>

const google::protobuf::FileDescriptor* init_protobuf(std::string message_def_str, std::string message_type) {
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