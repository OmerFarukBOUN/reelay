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

int main(int argc, char *argv[]) {
  zenoh::Config test_pub_config = zenoh::Config::create_default();
  auto session = zenoh::Session::open(std::move(test_pub_config));

  auto pub = session.declare_publisher(zenoh::KeyExpr("keyexpr/test"));

  while (true) {
    printf("Sending data\n");
    std::this_thread::sleep_for(1s);
    pub.put(R"({"test": true})");
  }
}