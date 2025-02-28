#include "reelay/monitors.hpp"
#include "zenoh.hxx"
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace std::chrono_literals;

using time_type = int64_t;
using input_type = reelay::json;
using output_type = reelay::json;
using config_type = reelay::json;

void data_handler(const zenoh::Sample &sample);
void data_handler2(const zenoh::Sample &sample);
bool generate_monitor_node(std::string pub_keyexpr, config_type pub_config,
                           std::string sub_keyexpr, config_type sub_config,
                           config_type monitor_config);

int main() {
  config_type pub_config = {};
  config_type sub_config = {};
  config_type monitor_config = reelay::json::parse(R"( {
    "monitor_type": "discrete",
    "pattern": "historically[0:5]{test}"
    })");
  generate_monitor_node("output/1", pub_config, "keyexpr/**", sub_config,
                        monitor_config);
}

reelay::monitor<input_type, output_type> *generated_monitor;
zenoh::Publisher *publisher_pnt;

// key_expr: keyexpr/test
// express:0
// payload:{"test": true}
// 0
// bruh2
// bruh3
// true
void data_handler(const zenoh::Sample &sample) {
  std::cout << "key_expr: " << sample.get_keyexpr().as_string_view()
            << std::endl;
  std::cout << "express:" << sample.get_express() << std::endl;
  // std::cout << "attachment:"
  //           << sample.get_attachment().value().get().as_string() <<
  //           std::endl;
  std::cout << "payload:" << sample.get_payload().as_string() << std::endl;
  std::cout << "" << sample.get_congestion_control() << std::endl;
  auto message = reelay::json::parse(sample.get_payload().as_string());
  std::cout << "bruh2" << std::endl;
  auto result = generated_monitor->update(message);
  std::cout << "bruh3" << std::endl;
  std::cout << result["value"] << std::endl;
  publisher_pnt->put(result.dump());
}

inline bool generate_monitor_node(std::string pub_keyexpr,
                                  config_type pub_config,
                                  std::string sub_keyexpr,
                                  config_type sub_config,
                                  config_type monitor_config) {

  // Initialize the subscriber node
  zenoh::Config sub_config_def = zenoh::Config::create_default();
  for (auto &option : sub_config.items()) {
    sub_config_def.insert_json5(option.key(), option.value().dump());
  }
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));
  auto subscriber = sub_session.declare_subscriber(
      zenoh::KeyExpr(sub_keyexpr), &data_handler, zenoh::closures::none);

  // Initialize the monitor
  if (monitor_config["monitor_type"] == "discrete") {
    auto options =
        reelay::discrete_timed<time_type>::monitor<input_type,
                                                   output_type>::options()
            .disable_condensing();
    generated_monitor = new reelay::monitor<input_type, output_type>(
        reelay::make_monitor(monitor_config["pattern"], options));
  } else if (monitor_config["monitor_type"] == "dense") {
    auto options = reelay::dense_timed<double>::monitor<input_type,
                                                        output_type>::options();
    generated_monitor = new reelay::monitor<input_type, output_type>(
        reelay::make_monitor(monitor_config["pattern"], options));
  } else {
    return false;
  }

  // Initialize the publisher node
  zenoh::Config pub_config_def = zenoh::Config::create_default();
  for (auto &option : pub_config.items()) {
    pub_config_def.insert_json5(option.key(), option.value().dump());
  }

  // Ini
  auto pub_session = zenoh::Session::open(std::move(pub_config_def));
  auto publisher = pub_session.declare_publisher(zenoh::KeyExpr(pub_keyexpr));
  publisher_pnt = &publisher;
  while (true) {
    std::this_thread::sleep_for(1s);
  }
  return true;
}
