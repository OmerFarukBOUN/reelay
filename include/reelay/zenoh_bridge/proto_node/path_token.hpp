#pragma once

#include <string>

typedef struct path_token {
	std::string key;
	int array_no;
  
	bool operator==(const path_token& other) const
	{
	  return key == other.key && array_no == other.array_no;
	}
  } path_token;
  
  namespace std {
  template<>
  struct hash<path_token> {
	std::size_t operator()(const path_token& obj) const
	{
	  return std::hash<std::string>()(obj.key) ^ std::hash<int>()(obj.array_no);
	}
  };
  }  // namespace std