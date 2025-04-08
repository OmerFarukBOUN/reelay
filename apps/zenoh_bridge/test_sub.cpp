#include "reelay/monitors.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "zenoh.hxx"
#include <stdio.h>
// #include "reelay/zenoh_bridge/proto_node.hpp"
// #include "reelay/zenoh_bridge/zenoh_pub.hpp"

using namespace std::chrono_literals;

using time_type = int64_t;
using output_type = reelay::json;
using config_type = reelay::json;
uint64_t total_time = 0;
int how_much = 0;

void data_handler2(const zenoh::Sample& sample)
{
  auto now = std::chrono::high_resolution_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                     now.time_since_epoch())
                     .count();
  total_time += (timestamp & 0xFFFFFFFF);
  how_much++;
  std::cout << "Receive " << how_much << " timestamp: " << total_time
            << " microseconds" << std::endl;

  std::cout << sample.get_payload().as_string() << std::endl;
}

int main(int argc, char* argv[])
{
  zenoh::Config sub_config_def = zenoh::Config::create_default();
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));
  auto subscriber = sub_session.declare_subscriber(
    zenoh::KeyExpr("output/**"), &data_handler2, zenoh::closures::none);
  while(true) {
    std::this_thread::sleep_for(1s);
  }
}
