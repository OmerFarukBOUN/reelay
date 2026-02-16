#!/usr/bin/env python3
"""
Converts Protocol Buffer (.proto) files to:
1. FlatBuffer schema (.fbs) files
2. C++ converter (.hpp) files for converting protobuf messages to flatbuffers
"""

import re
import os
from typing import List, Dict, Tuple, Optional
from dataclasses import dataclass, field


@dataclass
class EnumDef:
	name: str
	values: List[Tuple[str, int]]
	parent_path: List[str] = field(default_factory=list)


@dataclass
class FieldDef:
	name: str
	field_type: str
	number: int
	is_repeated: bool = False
	is_optional: bool = False


@dataclass
class MessageDef:
	name: str
	fields: List[FieldDef] = field(default_factory=list)
	nested_messages: List['MessageDef'] = field(default_factory=list)
	nested_enums: List[EnumDef] = field(default_factory=list)
	parent_path: List[str] = field(default_factory=list)


@dataclass
class ProtoFile:
	package: str
	imports: List[str] = field(default_factory=list)
	messages: List[MessageDef] = field(default_factory=list)
	enums: List[EnumDef] = field(default_factory=list)


class ProtoParser:
	def __init__(self, content: str):
		self.content = content
		self.lines = content.split('\n')
		
	def parse(self) -> ProtoFile:
		proto = ProtoFile(package="")
		
		# Remove comments and clean content
		cleaned_content = self._remove_comments(self.content)
		
		# Extract package
		package_match = re.search(r'package\s+([\w.]+)\s*;', cleaned_content)
		if package_match:
			proto.package = package_match.group(1)
		
		# Extract imports
		import_matches = re.finditer(r'import\s+"([^"]+)"\s*;', cleaned_content)
		for match in import_matches:
			proto.imports.append(match.group(1))
		
		# Parse messages
		proto.messages = self._parse_messages(cleaned_content, [])
		
		return proto
	
	def _remove_comments(self, content: str) -> str:
		# Remove single line comments
		content = re.sub(r'//.*', '', content)
		# Remove multi-line comments
		content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
		return content
	
	def _parse_messages(self, content: str, parent_path: List[str]) -> List[MessageDef]:
		messages = []
		
		# Find all message definitions using a more robust approach
		# Match 'message Name {' and then find the matching closing brace
		pattern = r'message\s+(\w+)\s*\{'
		
		pos = 0
		while True:
			match = re.search(pattern, content[pos:])
			if not match:
				break
			
			msg_name = match.group(1)
			start_pos = pos + match.end()
			
			# Find matching closing brace
			brace_count = 1
			i = start_pos
			while i < len(content) and brace_count > 0:
				if content[i] == '{':
					brace_count += 1
				elif content[i] == '}':
					brace_count -= 1
				i += 1
			
			if brace_count == 0:
				msg_body = content[start_pos:i-1]
				
				message = MessageDef(name=msg_name, parent_path=parent_path.copy())
				
				# Parse fields
				message.fields = self._parse_fields(msg_body)
				
				# Parse nested enums
				message.nested_enums = self._parse_enums(msg_body, parent_path + [msg_name])
				
				# Parse nested messages recursively
				message.nested_messages = self._parse_messages(msg_body, parent_path + [msg_name])
				
				messages.append(message)
				
				pos = i
			else:
				break
		
		return messages
	
	def _parse_fields(self, content: str) -> List[FieldDef]:
		fields = []
		
		# Remove nested message and enum blocks to avoid parsing their fields
		cleaned_content = self._remove_nested_blocks(content)
		
		# Match field definitions
		field_pattern = r'(optional|required|repeated)?\s*(\w+(?:\.\w+)*)\s+(\w+)\s*=\s*(\d+)\s*;'
		
		for match in re.finditer(field_pattern, cleaned_content):
			modifier = match.group(1)
			field_type = match.group(2)
			field_name = match.group(3)
			field_number = int(match.group(4))
			
			field = FieldDef(
				name=field_name,
				field_type=field_type,
				number=field_number,
				is_repeated=(modifier == 'repeated'),
				is_optional=(modifier == 'optional' or modifier is None)
			)
			fields.append(field)
		
		return fields
	
	def _remove_nested_blocks(self, content: str) -> str:
		"""Remove nested message and enum blocks from content"""
		result = content
		
		# Remove message blocks
		while True:
			match = re.search(r'message\s+\w+\s*\{', result)
			if not match:
				break
			
			start = match.start()
			brace_start = match.end() - 1
			
			# Find matching closing brace
			brace_count = 1
			i = brace_start + 1
			while i < len(result) and brace_count > 0:
				if result[i] == '{':
					brace_count += 1
				elif result[i] == '}':
					brace_count -= 1
				i += 1
			
			if brace_count == 0:
				result = result[:start] + result[i:]
			else:
				break
		
		# Remove enum blocks
		result = re.sub(r'enum\s+\w+\s*\{[^}]*\}', '', result)
		
		return result
	
	def _parse_enums(self, content: str, parent_path: List[str]) -> List[EnumDef]:
		enums = []
		
		# Remove nested message blocks first to avoid parsing their enums
		cleaned_content = self._remove_nested_message_blocks(content)
		
		# Find all enum definitions
		enum_pattern = r'enum\s+(\w+)\s*\{([^}]+)\}'
		
		for match in re.finditer(enum_pattern, cleaned_content):
			enum_name = match.group(1)
			enum_body = match.group(2)
			
			values = []
			value_pattern = r'(\w+)\s*=\s*(\d+)'
			for value_match in re.finditer(value_pattern, enum_body):
				values.append((value_match.group(1), int(value_match.group(2))))
			
			enum = EnumDef(name=enum_name, values=values, parent_path=parent_path.copy())
			enums.append(enum)
		
		return enums
	
	def _remove_nested_message_blocks(self, content: str) -> str:
		"""Remove only nested message blocks from content"""
		result = content
		
		# Remove message blocks
		while True:
			match = re.search(r'message\s+\w+\s*\{', result)
			if not match:
				break
			
			start = match.start()
			brace_start = match.end() - 1
			
			# Find matching closing brace
			brace_count = 1
			i = brace_start + 1
			while i < len(result) and brace_count > 0:
				if result[i] == '{':
					brace_count += 1
				elif result[i] == '}':
					brace_count -= 1
				i += 1
			
			if brace_count == 0:
				result = result[:start] + result[i:]
			else:
				break
		
		return result


class FlatBufferGenerator:
	def __init__(self, proto: ProtoFile):
		self.proto = proto
		self.output_lines = []
	
	def generate(self) -> str:
		self.output_lines = []
		
		# Add includes
		for import_path in self.proto.imports:
			fbs_import = import_path.replace('.proto', '.fbs')
			self.output_lines.append(f'include "{fbs_import}";')
		
		if self.proto.imports:
			self.output_lines.append('')
		
		# Process all messages (nested first)
		for message in self.proto.messages:
			self._generate_message(message, [])
		
		return '\n'.join(self.output_lines)
	
	def _generate_message(self, message: MessageDef, parent_path: List[str]):
		current_path = parent_path + [message.name]
		
		# Generate nested enums first
		for enum in message.nested_enums:
			self._generate_enum(enum, current_path)
		
		# Generate nested messages first (children before parent)
		for nested_msg in message.nested_messages:
			self._generate_message(nested_msg, current_path)
		
		# Generate the table
		# Build namespace: osi3 or osi3.ParentMsg_ (with underscore)
		if parent_path:
			namespace_parts = [self.proto.package]
			for part in parent_path:
				namespace_parts.append(f'{part}_')
			namespace = '.'.join(namespace_parts)
		else:
			namespace = self.proto.package
			
		self.output_lines.append(f'namespace {namespace};')
		self.output_lines.append(f'table {message.name} {{')
		
		for field in message.fields:
			field_line = self._generate_field(field, current_path)
			self.output_lines.append(f'  {field_line}')
		
		self.output_lines.append('}')
		self.output_lines.append('')
	
	def _generate_enum(self, enum: EnumDef, parent_path: List[str]):
		# Build namespace for enum: parent path with underscores, but NOT enum name
		namespace_parts = [self.proto.package]
		for part in parent_path:
			namespace_parts.append(f'{part}_')
		namespace = '.'.join(namespace_parts)
		
		self.output_lines.append(f'namespace {namespace};')
		self.output_lines.append(f'enum {enum.name} : int {{')
		
		for i, (name, value) in enumerate(enum.values):
			comma = ',' if i < len(enum.values) - 1 else ','
			self.output_lines.append(f'    {name} = {value}{comma}')
		
		self.output_lines.append('}')
		self.output_lines.append('')
	
	def _generate_field(self, field: FieldDef, current_path: List[str]) -> str:
		fbs_type = self._convert_type(field.field_type, current_path)
		
		if field.is_repeated:
			fbs_type = f'[{fbs_type}]'
		
		return f'{field.name}:{fbs_type};'
	
	def _convert_type(self, proto_type: str, current_path: List[str]) -> str:
		# Handle primitive types
		type_mapping = {
			'double': 'double',
			'float': 'float',
			'int32': 'int',
			'int64': 'long',
			'uint32': 'uint',
			'uint64': 'ulong',
			'sint32': 'int',
			'sint64': 'long',
			'fixed32': 'uint',
			'fixed64': 'ulong',
			'sfixed32': 'int',
			'sfixed64': 'long',
			'bool': 'bool',
			'string': 'string',
			'bytes': '[ubyte]'
		}
		
		if proto_type in type_mapping:
			return type_mapping[proto_type]
		
		# Check if type contains dots (fully qualified name like TrafficSign.MainSign.Classification.Type)
		if '.' in proto_type:
			# Split the qualified name and convert to FlatBuffer format
			parts = proto_type.split('.')
			# Build: osi3.TrafficSign_.MainSign_.Classification_.Type
			namespace_parts = [self.proto.package]
			for part in parts[:-1]:
				namespace_parts.append(f'{part}_')
			namespace_parts.append(parts[-1])
			return '.'.join(namespace_parts)
		
		# Check if it's a nested enum anywhere in the parent hierarchy
		enum_path = self._find_enum_path(proto_type, current_path)
		if enum_path:
			# Build full path for enum: osi3.ParentMsg_.ChildMsg_.EnumName
			namespace_parts = [self.proto.package]
			for part in enum_path[:-1]:
				namespace_parts.append(f'{part}_')
			namespace_parts.append(enum_path[-1])
			return '.'.join(namespace_parts)
		
		# Check if it's a nested message anywhere in the current path hierarchy
		nested_msg_path = self._find_nested_message_path(proto_type, current_path)
		if nested_msg_path:
			# Build full path: osi3.Parent1_.Parent2_.NestedMessage
			namespace_parts = [self.proto.package]
			for part in nested_msg_path[:-1]:
				namespace_parts.append(f'{part}_')
			namespace_parts.append(nested_msg_path[-1])
			return '.'.join(namespace_parts)
		
		# Handle custom types - build full namespace path
		return f'{self.proto.package}.{proto_type}'
	
	def _find_enum_path(self, enum_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Find the full path to an enum, searching in current message and parent hierarchy"""
		if len(current_path) == 0:
			return None
		
		# Check at each level of the hierarchy from current to root
		for i in range(len(current_path), 0, -1):
			check_path = current_path[:i]
			msg = self._find_message_by_path(check_path)
			if msg:
				for enum in msg.nested_enums:
					if enum.name == enum_name:
						return check_path + [enum_name]
		
		return None
	
	def _find_nested_message_path(self, msg_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Find the full path to a nested message, searching in current message hierarchy"""
		if len(current_path) == 0:
			return None
		
		# Check if it's a direct child of current message
		current_msg = self._find_message_by_path(current_path)
		if current_msg:
			for nested in current_msg.nested_messages:
				if nested.name == msg_name:
					return current_path + [msg_name]
		
		# Check at each level of the hierarchy
		for i in range(len(current_path), 0, -1):
			parent_path = current_path[:i]
			parent_msg = self._find_message_by_path(parent_path)
			if parent_msg:
				# Check direct children
				for nested in parent_msg.nested_messages:
					if nested.name == msg_name:
						return parent_path + [msg_name]
				# Check nested children recursively
				result = self._search_nested_message(parent_msg, msg_name, parent_path)
				if result:
					return result
		
		return None
	
	def _search_nested_message(self, message: MessageDef, target_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Recursively search for a nested message"""
		for nested in message.nested_messages:
			if nested.name == target_name:
				return current_path + [nested.name]
			# Search deeper
			result = self._search_nested_message(nested, target_name, current_path + [nested.name])
			if result:
				return result
		return None
	
	def _is_nested_message_in_path(self, msg_name: str, current_path: List[str]) -> bool:
		"""Check if a message is nested within the current or parent message"""
		if len(current_path) == 0:
			return False
		
		# Check at current level (for sibling messages or nested in same parent)
		parent_msg = self._find_message_by_path(current_path[:1])
		
		if parent_msg:
			for nested in parent_msg.nested_messages:
				if nested.name == msg_name:
					return True
		
		return False
	
	def _is_nested_enum_in_path(self, enum_name: str, current_path: List[str]) -> bool:
		"""Check if an enum is nested within the current message path"""
		# Find the message at current_path
		message = self._find_message_by_path(current_path)
		if message:
			for enum in message.nested_enums:
				if enum.name == enum_name:
					return True
		return False
	
	def _find_message_by_path(self, path: List[str]) -> Optional[MessageDef]:
		"""Find a message by its path"""
		if not path:
			return None
		
		# Start with top-level messages
		messages = self.proto.messages
		current_msg = None
		
		for i, name in enumerate(path):
			found = False
			for msg in messages:
				if msg.name == name:
					current_msg = msg
					messages = msg.nested_messages
					found = True
					break
			if not found:
				return None
		
		return current_msg
	
	def _build_namespace(self, path: List[str]) -> str:
		if not path:
			return self.proto.package
		
		namespace_parts = [self.proto.package]
		for part in path:
			namespace_parts.append(f'{part}_')
		
		return '.'.join(namespace_parts)


class CppConverterGenerator:
	def __init__(self, proto: ProtoFile, proto_filename: str):
		self.proto = proto
		self.proto_filename = proto_filename
		self.base_name = proto_filename.replace('.proto', '')
		self.output_lines = []
	
	def generate(self) -> str:
		self.output_lines = []
		
		# Add includes
		self.output_lines.append('#include <flatbuffers/flatbuffers.h>')
		self.output_lines.append('namespace fbs')
		self.output_lines.append('{')
		self.output_lines.append(f'#include "{self.base_name}_generated.h"')
		self.output_lines.append('}')
		self.output_lines.append('')
		self.output_lines.append(f'#include "{self.base_name}.pb.h"')
		self.output_lines.append('')
		self.output_lines.append('#include <vector>')
		
		# Add converter includes from imports
		if self.proto.imports:
			self.output_lines.append('')
			for import_path in self.proto.imports:
				converter_include = import_path.replace('.proto', '_converter.hpp')
				self.output_lines.append(f'#include "{converter_include}"')
		
		self.output_lines.append('')
		
		# Generate converter functions
		for message in self.proto.messages:
			self._generate_message_converter(message, [])
		
		return '\n'.join(self.output_lines)
	
	def _generate_message_converter(self, message: MessageDef, parent_path: List[str]):
		current_path = parent_path + [message.name]
		
		# Generate nested message converters first
		for nested_msg in message.nested_messages:
			self._generate_message_converter(nested_msg, current_path)
		
		# Generate function signature
		function_name = self._build_function_name(current_path)
		proto_type = self._build_proto_type(current_path)
		fbs_type = self._build_fbs_type(current_path)
		
		self.output_lines.append(f'// {"=" * 26}')
		self.output_lines.append(f'// {" - ".join(current_path)}')
		self.output_lines.append(f'// {"=" * 26}')
		self.output_lines.append('')
		self.output_lines.append(f'::flatbuffers::Offset<{fbs_type}> {function_name}(const {proto_type}& proto,')
		self.output_lines.append(f'{" " * len("::flatbuffers::Offset<> ")}flatbuffers::FlatBufferBuilder& builder)')
		self.output_lines.append('{')
		self.output_lines.append(f'    {fbs_type}Builder b(builder);')
		self.output_lines.append('    ')
		
		# Generate field conversions
		for field in message.fields:
			self._generate_field_conversion(field, current_path)
		
		self.output_lines.append('    return b.Finish();')
		self.output_lines.append('}')
		self.output_lines.append('')
	
	def _generate_field_conversion(self, field: FieldDef, current_path: List[str]):
		if field.is_repeated:
			# Handle repeated fields
			element_type = self._get_cpp_element_type(field.field_type, current_path)
			converter_func = self._get_converter_function_for_field(field, current_path)
			
			self.output_lines.append(f'    std::vector<::flatbuffers::Offset<{element_type}>> {field.name};')
			self.output_lines.append(f'    for (const auto& item : proto.{field.name}())')
			if converter_func == "Convertstring":
				self.output_lines.append(f'        {field.name}.push_back(builder.CreateString(item));')
			else:
				self.output_lines.append(f'        {field.name}.push_back({converter_func}(item, builder));')
			
			self.output_lines.append(f'    b.add_{field.name}(builder.CreateVector({field.name}));')
			self.output_lines.append('    ')
		else:
			# Handle singular fields
			self.output_lines.append(f'    if (proto.has_{field.name}())')
			
			if self._is_primitive_type(field.field_type):
				self.output_lines.append(f'    \tb.add_{field.name}(proto.{field.name}());')
			elif field.field_type == 'string':
				self.output_lines.append(f'    \tb.add_{field.name}(builder.CreateString(proto.{field.name}()));')
			elif self._is_enum_type(field.field_type, current_path):
				enum_type = self._build_fbs_enum_type(field.field_type, current_path)
				self.output_lines.append(f'    \tb.add_{field.name}(static_cast<{enum_type}>(proto.{field.name}()));')
			else:
				converter_func = self._get_converter_function_for_field(field, current_path)
				self.output_lines.append(f'    \tb.add_{field.name}({converter_func}(proto.{field.name}(), builder));')
			
			self.output_lines.append('    ')
	
	def _is_primitive_type(self, type_name: str) -> bool:
		primitives = ['double', 'float', 'int32', 'int64', 'uint32', 'uint64',
					 'sint32', 'sint64', 'fixed32', 'fixed64', 'sfixed32', 
					 'sfixed64', 'bool']
		return type_name in primitives
	
	def _is_enum_type(self, type_name: str, current_path: List[str]) -> bool:
		# Check if type contains dots (fully qualified name) - could be an enum
		if '.' in type_name:
			return True  # Assume fully qualified types can be enums, will handle in build
		
		# Check if type is a nested enum in current message or parent hierarchy
		enum_path = self._find_enum_path_for_cpp(type_name, current_path)
		return enum_path is not None
	
	def _find_enum_path_for_cpp(self, enum_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Find the full path to an enum for C++ type resolution"""
		if len(current_path) == 0:
			return None
		
		# Check at each level of the hierarchy from current to root
		for i in range(len(current_path), 0, -1):
			check_path = current_path[:i]
			msg = self._find_message_by_path(check_path)
			if msg:
				for enum in msg.nested_enums:
					if enum.name == enum_name:
						return check_path + [enum_name]
		
		return None
	
	def _check_enum_in_message(self, message: MessageDef, enum_name: str, search_path: List[str]) -> bool:
		msg_path = message.parent_path + [message.name]
		
		if msg_path == search_path:
			for enum in message.nested_enums:
				if enum.name == enum_name:
					return True
		
		for nested in message.nested_messages:
			if self._check_enum_in_message(nested, enum_name, search_path):
				return True
		
		return False
	
	def _build_fbs_enum_type(self, enum_name: str, current_path: List[str]) -> str:
		# Check if it's a fully qualified name with dots
		if '.' in enum_name:
			# Split and build: TrafficSign.MainSign.Type -> fbs::osi3::TrafficSign_::MainSign_::Type
			parts = enum_name.split('.')
			cpp_parts = ['fbs', self.proto.package]
			for part in parts[:-1]:
				cpp_parts.append(f'{part}_')
			cpp_parts.append(parts[-1])
			return '::'.join(cpp_parts)
		
		# Find the enum path in the hierarchy
		enum_path = self._find_enum_path_for_cpp(enum_name, current_path)
		if enum_path:
			# Build FBS enum type: fbs::osi3::ParentMsg_::ChildMsg_::EnumName
			namespace_parts = ['fbs', self.proto.package]
			for part in enum_path[:-1]:
				namespace_parts.append(f'{part}_')
			namespace_parts.append(enum_path[-1])
			return '::'.join(namespace_parts)
		
		# Fallback (shouldn't reach here)
		namespace_parts = ['fbs', self.proto.package]
		for part in current_path:
			namespace_parts.append(f'{part}_')
		namespace_parts.append(enum_name)
		return '::'.join(namespace_parts)
	
	def _get_cpp_element_type(self, proto_type: str, current_path: List[str] = None) -> str:
		if proto_type in ['string']:
			return '::flatbuffers::String'
		
		# Check if type contains dots (fully qualified name like TrafficSign.MainSign.Classification.Type)
		if '.' in proto_type:
			# Split the qualified name and convert to C++ format
			parts = proto_type.split('.')
			# Build: fbs::osi3::TrafficSign_::MainSign_::Classification_::Type
			cpp_parts = ['fbs', self.proto.package]
			for part in parts[:-1]:
				cpp_parts.append(f'{part}_')
			cpp_parts.append(parts[-1])
			return '::'.join(cpp_parts)
		
		# Check if it's a nested message
		if current_path:
			nested_msg_path = self._find_nested_message_path_for_cpp(proto_type, current_path)
			if nested_msg_path:
				# Build C++ type: fbs::osi3::Parent_::Child_::Message
				cpp_parts = ['fbs', self.proto.package]
				for part in nested_msg_path[:-1]:
					cpp_parts.append(f'{part}_')
				cpp_parts.append(nested_msg_path[-1])
				return '::'.join(cpp_parts)
		
		return f'fbs::{self.proto.package}::{proto_type}'
	
	def _find_nested_message_path_for_cpp(self, msg_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Find the full path to a nested message for C++ type resolution"""
		if len(current_path) == 0:
			return None
		
		# Check if it's a direct child of current message
		current_msg = self._find_message_by_path(current_path)
		if current_msg:
			for nested in current_msg.nested_messages:
				if nested.name == msg_name:
					return current_path + [msg_name]
		
		# Check at each level of the hierarchy
		for i in range(len(current_path), 0, -1):
			parent_path = current_path[:i]
			parent_msg = self._find_message_by_path(parent_path)
			if parent_msg:
				# Check direct children
				for nested in parent_msg.nested_messages:
					if nested.name == msg_name:
						return parent_path + [msg_name]
				# Check nested children recursively
				result = self._search_nested_message_cpp(parent_msg, msg_name, parent_path)
				if result:
					return result
		
		return None
	
	def _search_nested_message_cpp(self, message: MessageDef, target_name: str, current_path: List[str]) -> Optional[List[str]]:
		"""Recursively search for a nested message"""
		for nested in message.nested_messages:
			if nested.name == target_name:
				return current_path + [nested.name]
			# Search deeper
			result = self._search_nested_message_cpp(nested, target_name, current_path + [nested.name])
			if result:
				return result
		return None
	
	def _get_converter_function(self, proto_type: str) -> str:
		return f'Convert{proto_type}'
	
	def _get_converter_function_for_field(self, field: FieldDef, current_path: List[str]) -> str:
		"""Get the converter function name, handling nested types correctly"""
		# Check if type contains dots (fully qualified name)
		if '.' in field.field_type:
			# Split and build converter name: TrafficSign.MainSign.Type -> ConvertTrafficSignMainSignType
			parts = field.field_type.split('.')
			return 'Convert' + ''.join(parts)
		
		# Check if it's a nested message anywhere in the hierarchy
		nested_msg_path = self._find_nested_message_path_for_cpp(field.field_type, current_path)
		if nested_msg_path:
			# Build function name from full path: ConvertLaneClassificationRoadCondition
			return 'Convert' + ''.join(nested_msg_path)
		
		return f'Convert{field.field_type}'
	
	def _find_message_by_path(self, path: List[str]) -> Optional['MessageDef']:
		"""Find a message by its path"""
		if not path:
			return None
		
		# Start with top-level messages
		messages = self.proto.messages
		current_msg = None
		
		for i, name in enumerate(path):
			found = False
			for msg in messages:
				if msg.name == name:
					current_msg = msg
					messages = msg.nested_messages
					found = True
					break
			if not found:
				return None
		
		return current_msg
	
	def _build_function_name(self, path: List[str]) -> str:
		return 'Convert' + ''.join(path)
	
	def _build_proto_type(self, path: List[str]) -> str:
		parts = [self.proto.package] + ['::'.join(path)]
		return '::'.join(parts)
	
	def _build_fbs_type(self, path: List[str]) -> str:
		namespace_parts = ['fbs', self.proto.package]
		
		for i, part in enumerate(path[:-1]):
			namespace_parts.append(f'{part}_')
		
		namespace_parts.append(path[-1])
		
		return '::'.join(namespace_parts)


def convert_proto_file(proto_file_path: str, output_dir: str = None):
	"""
	Convert a .proto file to .fbs and .hpp files
	
	Args:
		proto_file_path: Path to the input .proto file
		output_dir: Directory to write output files (default: same as input)
	"""
	if output_dir is None:
		output_dir = os.path.dirname(proto_file_path) or '.'
	
	# Read proto file
	with open(proto_file_path, 'r') as f:
		content = f.read()
	
	# Parse proto file
	parser = ProtoParser(content)
	proto = parser.parse()
	
	# Get base filename
	base_name = os.path.basename(proto_file_path).replace('.proto', '')
	proto_filename = os.path.basename(proto_file_path)
	
	# Generate FlatBuffer schema
	fbs_generator = FlatBufferGenerator(proto)
	fbs_content = fbs_generator.generate()
	
	fbs_output_path = os.path.join(output_dir, f'{base_name}.fbs')
	with open(fbs_output_path, 'w') as f:
		f.write(fbs_content)
	
	print(f'Generated: {fbs_output_path}')
	
	# Generate C++ converter
	cpp_generator = CppConverterGenerator(proto, proto_filename)
	cpp_content = cpp_generator.generate()
	
	cpp_output_path = os.path.join(output_dir, f'{base_name}_converter.hpp')
	with open(cpp_output_path, 'w') as f:
		f.write(cpp_content)
	
	print(f'Generated: {cpp_output_path}')


if __name__ == '__main__':
	import sys
	
	if len(sys.argv) < 2:
		print('Usage: python proto_to_flatbuffer_converter.py <proto_file> [output_dir]')
		sys.exit(1)
	
	proto_file = sys.argv[1]
	output_dir = sys.argv[2] if len(sys.argv) > 2 else None
	
	convert_proto_file(proto_file, output_dir)