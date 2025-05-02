#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>
#include <stdio.h>

#include <boost/url.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// #include "proto_node.hpp"
#include "reelay/datafield.hpp"
#include "reelay/monitors.hpp"
#include "reelay/zenoh_bridge/globals.hpp"
#include "reelay/parser/ptl.hpp"
#include "zenoh.hxx"

using namespace std::chrono_literals;
using google::protobuf::FieldDescriptor;
using google::protobuf::Message;

using time_type = int64_t;
using output_type = reelay::json;
using config_type = reelay::json;
using message_type = std::variant<std::string, int32_t, int64_t, uint32_t,
                                  uint64_t, float, double, bool>;
using input_type = std::unordered_map<std::string, message_type>;

inline reelay::monitor<input_type, output_type> *zenoh_monitor;
inline zenoh::Publisher *publisher_pnt;
void data_handler(const zenoh::Sample &sample);

#include <chrono>
#include <cstdint>
#include <iostream>

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::microseconds;

// Accumulators for total time and count
static std::uint64_t count = 0;
static Duration total_proto_map_update{0};
static Duration total_monitor_update{0};

void data_handler(const zenoh::Sample &sample) {
    // std::cout << count << std::endl;
    // First update: proto_mapper
    auto t0 = Clock::now();
    global_proto_mapper->update(sample.get_payload().as_vector());
    auto t1 = Clock::now();
    total_proto_map_update += std::chrono::duration_cast<Duration>(t1 - t0);

    // Second update: reelay monitor
    auto t2 = Clock::now();
    auto result = zenoh_monitor->update(proto_map);
    auto t3 = Clock::now();
    total_monitor_update += std::chrono::duration_cast<Duration>(t3 - t2);

    publisher_pnt->put(result.dump());

    // Increment count and (optionally) report every N calls or at the end
    count++;
    if (count == 350) {
        double mean_proto = total_proto_map_update.count() / double(count);
        double mean_mon   = total_monitor_update.count()  / double(count);
        std::cout << "After " << count << " calls:\n"
                  << "  – mean proto_mapper->update(): "
                  << mean_proto << " µs\n"
                  << "  – mean zenoh_monitor->update(): "
                  << mean_mon << " µs\n" << std::endl;
    }
}
