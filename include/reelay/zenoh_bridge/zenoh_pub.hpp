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
#include <fstream>

using namespace std::chrono_literals;
using google::protobuf::FieldDescriptor;
using google::protobuf::Message;

using time_type = int64_t;
using output_type = reelay::json;
using config_type = reelay::json;
using json = reelay::json;
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
static Duration total_json_dump{0};
static Duration total_zenoh_put{0};

std::vector<std::chrono::microseconds::rep> receive_times;
std::vector<std::chrono::microseconds::rep> send_times;

void data_handler(const zenoh::Sample &sample) {
    // Record the time when data is received
    auto t0 = Clock::now();
    auto receive_time = std::chrono::duration_cast<Duration>(t0.time_since_epoch()).count();
    receive_times.push_back(receive_time);

    // First update: proto_mapper
    global_proto_mapper->update(sample.get_payload().as_vector());
    auto t1 = Clock::now();
    total_proto_map_update += std::chrono::duration_cast<Duration>(t1 - t0);

    // Second update: reelay monitor
    auto t2 = Clock::now();
    auto result = zenoh_monitor->update(proto_map);
    auto t3 = Clock::now();
    total_monitor_update += std::chrono::duration_cast<Duration>(t3 - t2);

    // Convert result to JSON string
    auto t4 = Clock::now();
    std::string json_string = result.dump();
    auto t5 = Clock::now();
    total_json_dump += std::chrono::duration_cast<Duration>(t5 - t4);

    // Record the time when data is published
    auto t6 = Clock::now();
    auto send_time = std::chrono::duration_cast<Duration>(t6.time_since_epoch()).count();
    send_times.push_back(send_time);

    // Publish the data
    // publisher_pnt->put(json_string);
    auto t7 = Clock::now();
    total_zenoh_put += std::chrono::duration_cast<Duration>(t7 - t6);

    // Increment count
    count++;

    // Save data to JSON files when count reaches 667
    if (count == 667) {
        // Save sender times
        json sender_json = json::array();
        for (size_t i = 0; i < send_times.size(); ++i) {
            sender_json.push_back({{"package_number", static_cast<int>(i)}, {"send_time", send_times[i]}});
        }
        std::ofstream sender_file("/shared_data/sender.json");
        sender_file << sender_json.dump(4); // Save with indentation
        sender_file.close();

        // Save receiver times
        json receiver_json = json::array();
        for (size_t i = 0; i < receive_times.size(); ++i) {
            receiver_json.push_back({{"package_number", static_cast<int>(i)}, {"recieve_time", receive_times[i]}});
        }
        std::ofstream receiver_file("/shared_data/receiver.json");
        receiver_file << receiver_json.dump(4); // Save with indentation
        receiver_file.close();

        std::cout << "Saved sender and receiver data to JSON files." << std::endl;
    }
}