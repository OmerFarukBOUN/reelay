#include <iostream>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>

#include <google/protobuf/compiler/parser.h>

// Proto definition (can come from any IO source: disk, network, ...):
char text[] = "syntax = \"proto2\";\n"
  "message APIPort3 {"
  "required uint32 AppLedStateOn = 1;"
  "required uint32 PotiPercentage = 2;"
  "required uint32 VDD = 3;"
  "}";
std::string message_type("APIPort3");

int main() {
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  using namespace google::protobuf::compiler;

  ArrayInputStream raw_input(text, strlen(text));
  Tokenizer input(&raw_input, NULL);

  // Proto definition to a representation as used by the protobuf lib:
  /* FileDescriptorProto documentation:
   * A valid .proto file can be translated directly to a FileDescriptorProto
   * without any other information (e.g. without reading its imports).
   * */
  FileDescriptorProto file_desc_proto;
  Parser parser;
  if (!parser.Parse(&input, &file_desc_proto)) {
    std::cerr << "Failed to parse .proto definition:" << text;
    return -1;
  }

  // Set the name in file_desc_proto as Parser::Parse does not do this:
  if (!file_desc_proto.has_name()) {
    file_desc_proto.set_name(message_type);
  }

  // Construct our own FileDescriptor for the proto file:
  /* FileDescriptor documentation:
   * Describes a whole .proto file.  To get the FileDescriptor for a compiled-in
   * file, get the descriptor for something defined in that file and call
   * descriptor->file().  Use DescriptorPool to construct your own descriptors.
   * */
  google::protobuf::DescriptorPool pool;
  const google::protobuf::FileDescriptor* file_desc =
    pool.BuildFile(file_desc_proto);
  if (file_desc == NULL) {
    std::cerr << "Cannot get file descriptor from file descriptor proto"
      << file_desc_proto.DebugString();
    return -1;
  }

  // As a .proto definition can contain more than one message Type,
  // select the message type that we are interested in
  const google::protobuf::Descriptor* message_desc =
    file_desc->FindMessageTypeByName(message_type);
  if (message_desc == NULL) {
    std::cerr << "Cannot get message descriptor of message: " << message_type
      << ", DebugString(): " << file_desc->DebugString();
    return -2;
  }

  // Print fields that are defined in the proto definition:
  // Note that proto defintion field numbers start from 1, but we assume that
  // the proto definition uses increasing field numbering without any gaps
  for (uint8_t i = 1; i <= message_desc->field_count(); i++) {
     const FieldDescriptor* field = message_desc->FindFieldByNumber(i);
     if (field)
       std::cout << field->name() << ": " << field->type_name() << " ("
         << field->label() << ")" << std::endl;
     else
       std::cerr << "Error fieldDescriptor object is NULL, field_count() = "
         << message_desc->field_count() << std::endl;
  }

  // Create an empty Message object that will hold the result of deserializing
  // a byte array for the proto definition:
  google::protobuf::DynamicMessageFactory factory;
  const google::protobuf::Message* prototype_msg =
    factory.GetPrototype(message_desc); // prototype_msg is immutable
  if (prototype_msg == NULL) {
    std::cerr << "Cannot create prototype message from message descriptor";
    return -3;
  }

  google::protobuf::Message* mutable_msg = prototype_msg->New();
  if (mutable_msg == NULL) {
    std::cerr << "Failed in prototype_msg->New(); to create mutable message";
    return -4;
  }

  // Deserialize a binary buffer that contains a message that is described by
  // the proto definition:
  uint8_t buffer[] = {0x08, 0x00, 0x10, 0x64, 0x18, 0xF5, 0x2D};
  if (!mutable_msg->ParseFromArray(buffer, 7)) {
    std::cerr << "Failed to parse value in buffer";
  }

  //std::cout << mutable_msg->DebugString();

  // Use the reflection interface to examine the contents.
  const Reflection* reflection = mutable_msg->GetReflection();
  std::vector<const FieldDescriptor*> fields;
  reflection->ListFields(*mutable_msg, &fields);
  // Loop over the fields that are present in the deserialized object:
  for (auto field_it = fields.begin(); field_it != fields.end(); field_it++) {
    const FieldDescriptor* field = *field_it;
     if (field) {
       // For our APIPort3 proto definition all types are UInt32, so
       // we don't have to inspect field->type_name() here:
       uint32 value = reflection->GetUInt32(*mutable_msg, field);
       std::cout << field->name() << " -> " << value << std::endl;
     } else
       std::cerr << "Error fieldDescriptor object is NULL" << std::endl;
  }

  return 0;
}
