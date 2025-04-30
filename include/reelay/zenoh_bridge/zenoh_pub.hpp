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

void data_handler(const zenoh::Sample &sample) {
    // std::cout << "bruh" << std::endl;
//     for (uint8_t byte : sample.get_payload().as_vector()) {
//       // static_cast<int> converts the byte to an integer to avoid printing a character
//       std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
//                 << static_cast<int>(byte) << " ";
//   }
    global_proto_mapper->update(sample.get_payload().as_vector());
    // std::cout << "bruh" << std::endl;
    // for (const auto& [key, value] : proto_map) {
    //     std::cout << key << ": ";
    //     std::visit([](auto&& arg) { 
    //         std::cout << "(" << typeid(arg).name() << ") " << arg; 
    //     }, value);
    //     std::cout << std::endl;
    // }
    auto result = zenoh_monitor->update(proto_map);
    // std::cout << "bruh2" << std::endl;
    // std::cout << result["value"] << std::endl;
    publisher_pnt->put(result.dump());
}
