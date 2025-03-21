/*
 * Copyright (c) 2019-2023 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "reelay/monitors.hpp"
#include "zenoh.hxx"
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>

#include "osi_common.pb.h"
#include "osi_object.pb.h"
#include "osi_sensorview.pb.h"
#include "osi_version.pb.h"

#include <google/protobuf/util/json_util.h>

namespace reelay {

template<typename time_t, typename input_t>
struct timefield {
};

template<typename input_t>
struct datafield {
};

template<typename T>
struct timefield<T, osi3::GroundTruth> {
  using input_t = osi3::GroundTruth;
  inline static T get_time(const input_t& container)
  {
    return static_cast<double>(container.mutable_timestamp()->seconds()) + 1E-9 * static_cast<double>(container.mutable_timestamp()->nanos());
  }
};

template<>
struct datafield<std::unordered_map<std::string, std::string>> {
  using input_t = std::unordered_map<std::string, std::string>;

  static std::unordered_set<std::string>& falsity()
  {
    static std::unordered_set<std::string> f = {"0", "false", "False"};
    return f;
  }

  inline static bool contains(const input_t& container, const std::string& key)
  {
    return container.find(key) != container.end();
  }

  inline static bool as_bool(const input_t& container, const std::string& key)
  {
    return falsity().find(container.at(key)) == falsity().end();
  }

  inline static int as_integer(const input_t& container, const std::string& key)
  {
    return std::stoi(container.at(key));
  }

  inline static double as_floating(
    const input_t& container, const std::string& key)
  {
    return std::stod(container.at(key));
  }

  inline static std::string as_string(
    const input_t& container, const std::string& key)
  {
    return container.at(key);
  }

  inline static bool contains(const input_t& container, std::size_t index)
  {
    throw std::runtime_error("");
  }

  inline static bool as_bool(const input_t& container, std::size_t index)
  {
    throw std::runtime_error("");
  }

  inline static int as_integer(const input_t& container, std::size_t index)
  {
    throw std::runtime_error("");
  }

  inline static double as_floating(const input_t& container, std::size_t index)
  {
    throw std::runtime_error("");
  }

  inline static std::string as_string(
    const input_t& container, std::size_t index)
  {
    throw std::runtime_error("");
  }
};

}  // namespace reelay
