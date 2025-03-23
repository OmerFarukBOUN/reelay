#include <stdio.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "reelay/monitors.hpp"
#include "reelay/zenoh_bridge/proto_node/proto_node.hpp"
#include "reelay/zenoh_bridge/zenoh_pub.hpp"
#include "reelay/zenoh_bridge/globals.hpp"
#include "zenoh.hxx"

using namespace std::chrono_literals;

using message_type = std::variant<std::string, int32_t, int64_t, uint32_t,
                                  uint64_t, float, double, bool>;
using time_type = int64_t;
using input_type = std::unordered_map<std::string, message_type>;
using output_type = reelay::json;
using config_type = reelay::json;

int main() {
    std::cout << "Hello, World!" << std::endl;
    zenoh::Config sub_config_def = zenoh::Config::create_default();
    auto sub_session = zenoh::Session::open(std::move(sub_config_def));
    auto subscriber = sub_session.declare_subscriber(
        zenoh::KeyExpr("keyexpr/**"), &data_handler, zenoh::closures::none);

    // Initialize the monitor
    char text[] =
        "syntax = \"proto2\";\n"
        "message APIPort3 {"
        "required uint32 AppLedStateOn = 1;"
        "required uint32 PotiPercentage = 2;"
        "required uint32 VDD = 3;"
        "}";
    std::string message_type("APIPort3");
    std::cout << "first" << std::endl;
    global_proto_mapper = new proto_mapper(text, message_type);
    auto options =
        reelay::discrete_timed<time_type>::monitor<input_type,
                                                   output_type>::options()
            .disable_condensing();
    std::cout << "monitor initialized" << std::endl;
    zenoh_monitor = new reelay::monitor<input_type, output_type>(
        reelay::make_monitor("historically[0:5]{#/AppLedStateOn: 2}", options));
    std::cout << "monitor initialized" << std::endl;
    zenoh::Config test_pub_config = zenoh::Config::create_default();
    auto session = zenoh::Session::open(std::move(test_pub_config));

    auto publisher = session.declare_publisher(zenoh::KeyExpr("out/out1"));
    publisher_pnt = &publisher;
    while (true) {
        std::this_thread::sleep_for(1s);
    }
}
