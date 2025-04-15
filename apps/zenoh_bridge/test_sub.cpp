#include "reelay/monitors.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "zenoh.hxx"
#include <stdio.h>
// #include "nlohmann/json.hpp"
// #include "reelay/zenoh_bridge/proto_node.hpp"
// #include "reelay/zenoh_bridge/zenoh_pub.hpp"

using namespace std::chrono_literals;

using time_type = int64_t;
using output_type = reelay::json;
using config_type = reelay::json;
std::chrono::steady_clock::time_point last_input_time;
uint64_t total_time = 0;
int how_much = 0;
// nlohmann::json json_data;

void data_handler2(const zenoh::Sample& sample)
{

  auto now = std::chrono::high_resolution_clock::now();
  last_input_time = std::chrono::steady_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                     now.time_since_epoch())
                     .count();
  how_much++;
  // json_data[how_much] = timestamp;
  std::cout << sample.get_payload().as_string() << std::endl;
}

int main(int argc, char* argv[])
{
  zenoh::Config sub_config_def = zenoh::Config::create_default();
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));
  auto subscriber = sub_session.declare_subscriber(
    zenoh::KeyExpr("output/**"), &data_handler2, zenoh::closures::none);
  std::thread timeout_thread([&]() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      auto current_time = std::chrono::steady_clock::now();
      auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_input_time).count();
      if (elapsed_time > 10) {
        std::cout << "No input for 10 seconds, exiting..." << std::endl;
        exit(0);
      }
    }
  });
}
