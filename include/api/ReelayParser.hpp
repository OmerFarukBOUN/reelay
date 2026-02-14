#pragma once

#include <unordered_map>

#include <reelay/pub_sub_connection/globals.hpp>
#include <reelay/pub_sub_connection/path_token.hpp>

template<typename InMsg, typename MonitorMsg>
class ReelayParser {
 public:
  ReelayParser() : path2token_map(global_path2token_map) {}

  virtual ~ReelayParser() = default;

  virtual MonitorMsg parse(const InMsg& in) = 0;

 protected:
  std::unordered_map<std::vector<path_token>, std::string> path2token_map;
};
