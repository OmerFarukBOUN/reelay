#pragma once
#include "reelay/pub_sub_connection/path_token.hpp"

#include <variant>

using message_type = std::variant<
  std::string,
  int32_t,
  int64_t,
  uint32_t,
  uint64_t,
  float,
  double,
  bool>;

// Bu yöntemi Hoca ile konuşup monitörün bir field ı olarak halledeceğiz
inline std::unordered_map<std::vector<path_token>, std::string>
  global_path2token_map;
inline std::unordered_map<std::string, message_type> global_token2value_map;