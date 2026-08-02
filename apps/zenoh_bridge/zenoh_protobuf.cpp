#include "reelay/monitors.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <stdio.h>

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

#include "base/LTLMonitoringPipeline.hpp>
#include <apis/protobuf/ProtobufParser.hpp>
#include <apis/protobuf/converters.hpp>
#include <apis/protobuf/proto_define.hpp>
#include <apis/zenoh/ZenohPublisher.hpp>
#include <apis/zenoh/ZenohSubscriber.hpp>


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
  init_protobufs();
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
  std::cout << "pub_keyexpr: " << pub_keyexpr << std::endl;
  std::cout << "sub_keyexpr: " << sub_keyexpr << std::endl;
  std::cout << "monitor_pattern: " << monitor_pattern << std::endl;
  std::cout << "message_type: " << message_type << std::endl;
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
  std::string message_type_s)
{
  using SubMsg = std::vector<uint8_t>;
  using MonitorMsg = std::unordered_map<std::string, message_type>;
  using PubMsg = reelay::json;

  zenoh::Config sub_config_def = zenoh::Config::create_default();
  for(auto& option : sub_config.items()) {
    sub_config_def.insert_json5(option.key(), option.value().dump());
  }
  auto sub_session = zenoh::Session::open(std::move(sub_config_def));

  zenoh::Config pub_config_def = zenoh::Config::create_default();
  for(auto& option : pub_config.items()) {
    pub_config_def.insert_json5(option.key(), option.value().dump());
  }
  auto pub_session = zenoh::Session::open(std::move(pub_config_def));

  auto options = reelay::discrete_timed<
                   time_type>::monitor<input_type, output_type>::options()
                   .disable_condensing();
  auto sub = new ZenohSubscriber(sub_session, sub_keyexpr);
  auto parser = new ProtobufParser(message_type_s);
  auto monitor = new reelay::monitor<input_type, output_type>(
    reelay::make_monitor(monitor_pattern, options));
  auto pub = new ZenohPublisher(pub_session, pub_keyexpr);
  LTLMonitoringPipeline<SubMsg, MonitorMsg, PubMsg>(sub, parser, monitor, pub);
  while(true) {
    std::this_thread::sleep_for(1s);
  }
  return true;
}