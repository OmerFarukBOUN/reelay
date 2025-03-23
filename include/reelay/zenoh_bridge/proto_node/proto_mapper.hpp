#pragma once

#include <reelay/zenoh_bridge/proto_node/proto_node.hpp>

class proto_mapper {
	public:
	 std::unordered_map<path_token, proto_node> children;
	 const google::protobuf::Message* prototype_msg;
   
	 proto_mapper() : prototype_msg(nullptr) {}
   
	 proto_mapper(std::string message_def_str, std::string message_type)
	 {
	   ArrayInputStream raw_input(message_def_str.c_str(), message_def_str.size());
	   Tokenizer input(&raw_input, NULL);
   
	   Parser parser;
	   if(!parser.Parse(&input, &file_desc_proto)) {
		 std::cerr << "Failed to parse .proto definition:" << message_def_str;
		 return;
	   }
	   if(!file_desc_proto.has_name()) {
		 file_desc_proto.set_name(message_type);
	   }
	   const google::protobuf::FileDescriptor* file_desc =
		 pool.BuildFile(file_desc_proto);
	   if(file_desc == NULL) {
		 std::cerr << "Cannot get file descriptor from file descriptor proto"
				   << file_desc_proto.DebugString();
		 return;
	   }
	   const google::protobuf::Descriptor* message_desc =
		 file_desc->FindMessageTypeByName(message_type);
	   if(message_desc == NULL) {
		 std::cerr << "Cannot get message descriptor of message: " << message_type
				   << ", DebugString(): " << file_desc->DebugString();
		 return;
	   }
	   prototype_msg =
		 factory.GetPrototype(message_desc);  // prototype_msg is immutable
	   if(prototype_msg == NULL) {
		 std::cerr << "Cannot create prototype message from message descriptor";
		 return;
	   }
	 }
   
	 void add(std::vector<path_token> path)
	 {
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
	   for(uint8_t byte : msg) {
		 // static_cast<int> converts the byte to an integer to avoid printing a
		 // character
		 std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
				   << static_cast<int>(byte) << " ";
	   }
	   std::cout << std::endl;
	   google::protobuf::Message* new_msg = prototype_msg->New();
	   // prototype_msg->GetDescriptor();
	   // children.at({"AppLedStateOn", -1}).message->GetDescriptor();
	   // std::cout << "control" << std::endl;
	   if(!new_msg->ParseFromArray(msg.data(), msg.size())) {
		 std::cerr << "Failed to parse value in buffer";
		 exit(EXIT_FAILURE);
	   }
	   // children.at({"AppLedStateOn", -1}).message->GetReflection();
	   // std::cout << mutable_msg->DebugString() << std::endl;
	   // std::cout << new_msg->DebugString() << std::endl;
	   for(auto& child : children) {
		 std::cout << "update children" << std::endl;
		 child.second.update(*new_msg);
		 std::cout << "update children bitti" << std::endl;
	   }
	 }
   };
