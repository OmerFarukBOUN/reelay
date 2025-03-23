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

bool generate_monitor_node(std::string pub_keyexpr, config_type pub_config,
                           std::string sub_keyexpr, config_type sub_config,
                           config_type monitor_config);

int main() {
    config_type pub_config = {};
    config_type sub_config = {};
    config_type monitor_config = reelay::json::parse(R"( {
    "monitor_type": "discrete",
    "pattern": "historically[0:5]{timestamp != 80}"
    })");
    generate_monitor_node("output/1", pub_config, "esmini/gt", sub_config,
                          monitor_config);
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
        auto options =
            reelay::dense_timed<double>::monitor<input_type,
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