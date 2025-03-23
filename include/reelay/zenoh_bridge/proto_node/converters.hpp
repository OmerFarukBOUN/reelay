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

static message_type convert_uint32(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetUInt32(msg, field);
  }
  
  static message_type convert_uint64(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetUInt64(msg, field);
  }
  
  static message_type convert_int32(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetInt32(msg, field);
  }
  
  static message_type convert_int64(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetInt64(msg, field);
  }
  
  static message_type convert_float(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetFloat(msg, field);
  }
  
  static message_type convert_double(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetDouble(msg, field);
  }
  
  static message_type convert_bool(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetBool(msg, field);
  }
  
  static message_type convert_string(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetString(msg, field);
  }
  
  static message_type convert_repeated_uint32(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedUInt32(msg, field, array_no);
  }
  
  static message_type convert_repeated_uint64(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedUInt64(msg, field, array_no);
  }
  
  static message_type convert_repeated_int32(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedInt32(msg, field, array_no);
  }
  
  static message_type convert_repeated_int64(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedInt64(msg, field, array_no);
  }
  
  static message_type convert_repeated_float(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedFloat(msg, field, array_no);
  }
  
  static message_type convert_repeated_double(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedDouble(msg, field, array_no);
  }
  
  static message_type convert_repeated_bool(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedBool(msg, field, array_no);
  }
  
  static message_type convert_repeated_string(
	const google::protobuf::Message& msg,
	const google::protobuf::FieldDescriptor* field,
	int array_no)
  {
	return msg.GetReflection()->GetRepeatedString(msg, field, array_no);
  }