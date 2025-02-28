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

void data_handler(const zenoh::Sample &sample) {
  std::cout << "bruh" << std::endl;
  std::cout << sample.get_payload().as_string() << std::endl;
}

int main(int argc, char *argv[]) {
  zenoh::Config sub_config_def = zenoh::Config::create_default();
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));
  auto subscriber = sub_session.declare_subscriber(
      zenoh::KeyExpr("output/**"), &data_handler, zenoh::closures::none);
  while (true) {
    std::this_thread::sleep_for(1s);
  }
}