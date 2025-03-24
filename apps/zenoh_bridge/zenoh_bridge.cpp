#include "reelay/monitors.hpp"
#include "reelay/zenoh_bridge/globals.hpp"
#include "reelay/zenoh_bridge/proto_node/proto_define.hpp"
#include "reelay/zenoh_bridge/proto_node/proto_node.hpp"
#include "reelay/zenoh_bridge/zenoh_pub.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "osi_common.pb.h"
#include "osi_datarecording.pb.h"
#include "osi_detectedlane.pb.h"
#include "osi_detectedobject.pb.h"
#include "osi_detectedoccupant.pb.h"
#include "osi_detectedroadmarking.pb.h"
#include "osi_detectedtrafficlight.pb.h"
#include "osi_detectedtrafficsign.pb.h"
#include "osi_environment.pb.h"
#include "osi_featuredata.pb.h"
#include "osi_groundtruth.pb.h"
#include "osi_hostvehicledata.pb.h"
#include "osi_lane.pb.h"
#include "osi_logicaldetectiondata.pb.h"
#include "osi_logicallane.pb.h"
#include "osi_motionrequest.pb.h"
#include "osi_object.pb.h"
#include "osi_occupant.pb.h"
#include "osi_referenceline.pb.h"
#include "osi_roadmarking.pb.h"
#include "osi_route.pb.h"
#include "osi_sensordata.pb.h"
#include "osi_sensorspecific.pb.h"
#include "osi_sensorview.pb.h"
#include "osi_sensorviewconfiguration.pb.h"
#include "osi_streamingupdate.pb.h"
#include "osi_trafficcommand.pb.h"
#include "osi_trafficcommandupdate.pb.h"
#include "osi_trafficlight.pb.h"
#include "osi_trafficsign.pb.h"
#include "osi_trafficupdate.pb.h"
#include "zenoh.hxx"
#include <stdio.h>

using namespace std::chrono_literals;

using message_type = std::variant<
  std::string,
  int32_t,
  int64_t,
  uint32_t,
  uint64_t,
  float,
  double,
  bool>;

using time_type = int64_t;
using input_type = std::unordered_map<std::string, message_type>;
using output_type = reelay::json;
using config_type = reelay::json;

bool generate_monitor_node(
  std::string pub_keyexpr,
  config_type pub_config,
  std::string sub_keyexpr,
  config_type sub_config,
  std::string monitor_pattern,
  std::string message_type);

int main()
{
  for(const auto& entry :
      std::filesystem::directory_iterator("/home/nonroot/protobufs")) {
    if(entry.path().extension() == ".proto") {
      std::ifstream file(entry.path());
      if(file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string name = entry.path().stem().string();
        init_protobuf(content, name);
      }
    }
  }
  std::ifstream settings_file("/home/nonroot/settings.json");
  if(!settings_file.is_open()) {
    std::cerr << "Failed to open settings.json" << std::endl;
    return 1;
  }
  auto settings = reelay::json::parse(settings_file);

  config_type pub_config = settings.contains("pub_config")
                             ? settings["pub_config"]
                             : reelay::json::object();
  config_type sub_config = settings.contains("sub_config")
                             ? settings["sub_config"]
                             : reelay::json::object();
  std::string monitor_pattern =
    settings.contains("monitor_pattern") ? settings["monitor_pattern"] : "true";
  std::string message_type = settings.contains("message_type")
                               ? settings["message_type"]
                               : "osi3.GroundTruth";
  std::string pub_keyexpr =
    settings.contains("pub_keyexpr") ? settings["pub_keyexpr"] : "output/1";
  std::string sub_keyexpr =
    settings.contains("sub_keyexpr") ? settings["sub_keyexpr"] : "esmini/gt";
  generate_monitor_node(
    pub_keyexpr,
    pub_config,
    sub_keyexpr,
    sub_config,
    monitor_pattern,
    message_type);
}

inline bool generate_monitor_node(
  std::string pub_keyexpr,
  config_type pub_config,
  std::string sub_keyexpr,
  config_type sub_config,
  std::string monitor_pattern,
  std::string message_type)
{
  // Initialize the subscriber node
  zenoh::Config sub_config_def = zenoh::Config::create_default();
  for(auto& option : sub_config.items()) {
    sub_config_def.insert_json5(option.key(), option.value().dump());
  }
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));

  // Initialize the monitor
  global_proto_mapper = new proto_mapper(message_type);
  auto options = reelay::discrete_timed<
                   time_type>::monitor<input_type, output_type>::options()
                   .disable_condensing();

  // Initialize the publisher node
  zenoh::Config pub_config_def = zenoh::Config::create_default();
  for(auto& option : pub_config.items()) {
    pub_config_def.insert_json5(option.key(), option.value().dump());
  }

  // Ini
  auto pub_session = zenoh::Session::open(std::move(pub_config_def));
  auto publisher = pub_session.declare_publisher(zenoh::KeyExpr(pub_keyexpr));
  publisher_pnt = &publisher;
  auto generated_monitor = new reelay::monitor<input_type, output_type>(
    reelay::make_monitor(monitor_pattern, options));
  auto subscriber = sub_session.declare_subscriber(
    zenoh::KeyExpr(sub_keyexpr), &data_handler, zenoh::closures::none);

  while(true) {
    std::this_thread::sleep_for(1s);
  }
  return true;
}