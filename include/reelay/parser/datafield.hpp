// /*
//  * Copyright (c) 2019-2023 Dogan Ulus
//  *
//  * This Source Code Form is subject to the terms of the Mozilla Public
//  * License, v. 2.0. If a copy of the MPL was not distributed with this
//  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
//  */

// #pragma once

// #include <stdexcept>
// #include <string>
// #include <unordered_map>
// #include <unordered_set>
// #include <variant>

// #include "reelay/datafield.hpp"

// using message_type = std::variant<std::string, int32_t, int64_t, uint32_t,
//                                   uint64_t, float, double, bool>;

// namespace reelay {

// template <typename T>
// struct timefield<T, std::unordered_map<std::string, message_type>> {
//     using input_t = std::unordered_map<std::string, message_type>;
//     inline static T get_time(const input_t& container) {
//         static T time = -1;
//         T++;
//         return T;
//     }
// };

// template <>
// struct datafield<std::unordered_map<std::string, message_type>> {
//     using input_t = std::unordered_map<std::string, message_type>;

//     static std::unordered_set<message_type>& falsity() {
//         static std::unordered_set<message_type> f = {"0", "false", "False", 0, false, 0.0f};
//         return f;
//     }

//     inline static bool contains(const input_t& container,
//                                 const std::string& key) {
//         return container.find(key) != container.end();
//     }

//     inline static bool as_bool(const input_t& container,
//                                const std::string& key) {
//         return falsity().find(container.at(key)) == falsity().end();
//     }

//     inline static int as_integer(const input_t& container,
//                                  const std::string& key) {
//         return std::get<int>(container.at(key));
//     }

//     inline static double as_floating(const input_t& container,
//                                      const std::string& key) {
//         return std::get<double>(container.at(key));
//     }

//     inline static std::string as_string(const input_t& container,
//                                         const std::string& key) {
//         return std::get<std::string>(container.at(key));
//     }

//     inline static bool contains(const input_t& container, std::size_t index) {
//         throw std::runtime_error("");
//     }

//     inline static bool as_bool(const input_t& container, std::size_t index) {
//         throw std::runtime_error("");
//     }

//     inline static int as_integer(const input_t& container, std::size_t index) {
//         throw std::runtime_error("");
//     }

//     inline static double as_floating(const input_t& container,
//                                      std::size_t index) {
//         throw std::runtime_error("");
//     }

//     inline static std::string as_string(const input_t& container,
//                                         std::size_t index) {
//         throw std::runtime_error("");
//     }
// };

// }  // namespace reelay
