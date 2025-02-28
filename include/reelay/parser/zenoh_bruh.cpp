#include "reelay/monitors.hpp"
#include "reelay/parser/ptl_inspector.hpp"
#include "zenoh.hxx"
#include <boost/url.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <unordered_map>
#include <vector>
#include "zenoh_node.hpp"
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std::chrono_literals;
using google::protobuf::Message;
using google::protobuf::FieldDescriptor;

using time_type = int64_t;
using input_type = reelay::json;
using output_type = reelay::json;
using config_type = reelay::json;
using message_type =std::variant<std::string, int32_t, int64_t, uint32_t, uint64_t, float, double, bool>;

reelay::monitor<input_type, output_type> *zenoh_monitor;
zenoh::Publisher *publisher_pnt;

void data_handler(const zenoh::Sample &sample);

std::unordered_map<std::string, message_type> monitor_map;



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
  auto result = zenoh_monitor->update(message);
  std::cout << "bruh3" << std::endl;
  std::cout << result["value"] << std::endl;
  publisher_pnt->put(result.dump());
}
