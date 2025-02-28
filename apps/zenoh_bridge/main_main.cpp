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

using namespace std::chrono_literals;

using time_type = int64_t;
using input_type = reelay::json;
using output_type = reelay::json;
using config_type = reelay::json;

int main(int argc, char *argv[]) {
  bool error = false;
  auto inspector = reelay::ptl_inspector();

  // Hashmap to store monitors
  std::unordered_map<std::string, reelay::monitor<input_type, output_type> *>
      monitors;
  // Check for filename argument
  if (argc != 2) {
    std::cout << "Error: Program requires a filename argument." << std::endl;
    return -1;
  }

  // Open configuration file
  std::ifstream f(argv[1]);

  // Check if file was opened successfully
  if (!f.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return -1;
  }

  // Parse configuration file
  config_type config;
  try {
    config = reelay::json::parse(f);
  } catch (const nlohmann::json::parse_error &e) {
    std::cerr << "Error: Failed to parse JSON file. " << e.what() << std::endl;
    return -1;
  }

  // Iterate through monitors in configuration file
  for (auto &[monitor_name, monitor_config] : config.items()) {
    // Check if monitor configuration contains a 'monitor_type' and 'pattern'
    // key
    if (!(monitor_config.contains("monitor_type") &
          monitor_config.contains("pattern"))) {
      std::cerr << "Error: Monitor configuration for " << monitor_name
                << " does not contain a "
                   "'monitor_type' or 'pattern' key."
                << std::endl;
      error = true;
      continue;
    }

    // Check if monitor pattern is in PTL format
    std::string pattern = monitor_config["pattern"];
    auto inspection = inspector.inspect(pattern);
    if (!reelay::any_cast<bool>(inspection["has_references"])) {
      std::cerr << "Error: Monitor pattern for " << monitor_name
                << " is not in PTL format." << std::endl;
      error = true;
      continue;
    }

    reelay::monitor<input_type, output_type> *generated_monitor;

    if (monitor_config["monitor_type"] == "discrete") {
      auto options =
          reelay::discrete_timed<time_type>::monitor<input_type,
                                                     output_type>::options()
              .disable_condensing();
      generated_monitor = new reelay::monitor<input_type, output_type>(
          reelay::make_monitor(monitor_config["pattern"], options));
    } else if (monitor_config["monitor_type"] == "dense") {
      auto options =
          reelay::dense_timed<time_type>::monitor<input_type,
                                                  output_type>::options();
      generated_monitor = new reelay::monitor<input_type, output_type>(
          reelay::make_monitor(monitor_config["pattern"], options));
    } else {
      // Error if monitor type is invalid
      std::cerr << "Error: Invalid monitor type in configuration file."
                << std::endl;
      error = true;
      continue;
    }
  }
  if (error) {
    return -1;
  }
}