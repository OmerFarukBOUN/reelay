#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct path_token {
    std::string key;
    int array_no;

    bool operator==(const path_token& other) const {
        return key == other.key && array_no == other.array_no;
    }
};

namespace std {

template<>
struct hash<path_token> {
    size_t operator()(const path_token& obj) const noexcept {
        size_t h1 = hash<std::string>()(obj.key);
        size_t h2 = hash<int>()(obj.array_no);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

template<>
struct hash<std::vector<path_token>> {
    size_t operator()(const std::vector<path_token>& vec) const noexcept {
        size_t seed = 0;
        hash<path_token> hasher;
        for (const auto& t : vec) {
            seed ^= hasher(t) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

}  // namespace std