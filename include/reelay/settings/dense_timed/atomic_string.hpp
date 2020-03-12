/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "functional"
#include "memory"
#include "vector"

#include "reelay/common.hpp"
#include "reelay/datafield.hpp"
#include "reelay/intervals.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T, typename K = std::string>
struct atomic_string : public dense_timed_state<X, interval_set<T>, T> {
  using key_t = K;
  using input_t = X;
  using time_t = T;

  using base_t = reelay::interval_set<time_t>;
  using output_t = reelay::interval_set<time_t>;

  using function_t =
      std::function<output_t(const input_t &, const input_t &, time_t, time_t)>;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  interval_set value = interval_set();
  key_t key;
  std::string constant;

  explicit atomic_string(const key_t &k, const std::string &c)
      : key(k), constant(c) {}

  explicit atomic_string(const kwargs &kw)
      : atomic_string(reelay::any_cast<key_t>(kw.at("key")),
                      reelay::any_cast<std::string>(kw.at("constant"))) {}

  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {

    std::string new_data;

    try {
      new_data = datafield<input_t>::as_string(args, key);
    } catch (const std::out_of_range &e) {
      return; // Do nothing if the key does not exist - existing value persists
    }

    value = value & interval::left_open(previous, now);
    if (new_data == constant) {
      value.add(interval::left_open(now, reelay::infinity<time_t>::value()));
    }
  }

  output_t output(time_t previous, time_t now) override {
    return value & interval::left_open(previous, now);
  }
};

} // namespace dense_timed_setting
} // namespace reelay