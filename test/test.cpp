// file: jsonl_to_zenoh.cpp
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <regex>
#include <string>
#include <vector>

// External libs
#include "reelay/monitors.hpp"  // https://github.com/nlohmann/json

#include <google/protobuf/util/json_util.h>  // for optional JSON<->Proto if needed
#include <yaml-cpp/yaml.h>                   // yaml-cpp

// Include your protobuf generated header (replace with the actual header
// name/path)
#include "pqrs.pb.h"  // <-- make sure this matches your generated file

// Zenoh C++ headers - adjust if your installation uses a different include
// path/names
#include "zenoh.hxx"  // depending on your zenoh-cpp install, this may differ

// User macros (string literals)
#define test_path "/timescales/fullsuite/"
#define settings_path "/home/nonroot/settings.json"
#define zenoh_bridge "/zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge"

namespace fs = std::filesystem;
using json = nlohmann::json;

// A thread-safe queue to collect incoming messages from Zenoh subscriber
class MessageCollector {
 public:
  void push(const std::string& msg)
  {
    {
      std::lock_guard<std::mutex> lk(mtx_);
      q_.push(msg);
    }
    cv_.notify_one();
  }

  // wait and pop one message; returns empty optional on timeout
  std::optional<std::string> wait_pop(
    std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
  {
    std::unique_lock<std::mutex> lk(mtx_);
    if(!cv_.wait_for(lk, timeout, [&] {
         return !q_.empty();
       })) {
      return std::nullopt;
    }
    std::string v = q_.front();
    q_.pop();
    return v;
  }

 private:
  std::queue<std::string> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

// Transform pattern: every {smt} => {#/smt}
std::string transform_pattern(const std::string& pattern)
{
  std::string result;
  result.reserve(pattern.size() * 2);
  std::regex re(R"(\{([^}]+)\})");
  std::smatch m;
  std::string s = pattern;
  std::string::const_iterator searchStart(s.cbegin());
  while(std::regex_search(searchStart, s.cend(), m, re)) {
    result.append(m.prefix().first, m.prefix().second);  // text before match
    std::string inner = m[1].str();
    result += "{#/" + inner + "}";
    searchStart = m.suffix().first;
  }
  result.append(searchStart, s.cend());
  return result;
}

// Read YAML file and extract "pattern" key (top-level)
std::optional<std::string> read_pattern_from_yaml(const fs::path& yaml_path)
{
  try {
    YAML::Node root = YAML::LoadFile(yaml_path.string());
    if(root["pattern"]) {
      if(root["pattern"].IsScalar()) {
        return root["pattern"].as<std::string>();
      }
      else {
        std::cerr << "pattern is not scalar in " << yaml_path << "\n";
        return std::nullopt;
      }
    }
    else {
      std::cerr << "no 'pattern' key in " << yaml_path << "\n";
      return std::nullopt;
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Failed to read YAML " << yaml_path << ": " << e.what()
              << "\n";
    return std::nullopt;
  }
}

// Update settings.json with monitor_pattern
bool update_settings_monitor_pattern(
  const std::string& settings_file, const std::string& monitor_pattern)
{
  try {
    json j;
    // Read existing if present
    std::ifstream ifs(settings_file);
    if(ifs.good()) {
      ifs >> j;
      ifs.close();
    }
    else {
      j = json::object();
    }
    j["monitor_pattern"] = monitor_pattern;
    std::ofstream ofs(settings_file);
    ofs << j.dump(4);
    ofs.close();
    return true;
  }
  catch(const std::exception& e) {
    std::cerr << "Failed to update settings file " << settings_file << ": "
              << e.what() << "\n";
    return false;
  }
}

// Convert single JSON object (nlohmann::json) into protobuf LTLtest
// Assumes LTLtest has optional bool p,q,r,s fields and setters like set_p(bool)
bool json_to_protobuf(const json& j, LTLtest& out)
{
  // Clear message first
  out.Clear();

  // p,q,r,s are optional. Only set if exist and are boolean.
  if(j.contains("p") && !j["p"].is_null()) {
    if(j["p"].is_boolean())
      out.set_p(j["p"].get<bool>());
    else {
      std::cerr << "warning: 'p' not boolean\n";
    }
  }
  if(j.contains("q") && !j["q"].is_null()) {
    if(j["q"].is_boolean())
      out.set_q(j["q"].get<bool>());
    else {
      std::cerr << "warning: 'q' not boolean\n";
    }
  }
  if(j.contains("r") && !j["r"].is_null()) {
    if(j["r"].is_boolean())
      out.set_r(j["r"].get<bool>());
    else {
      std::cerr << "warning: 'r' not boolean\n";
    }
  }
  if(j.contains("s") && !j["s"].is_null()) {
    if(j["s"].is_boolean())
      out.set_s(j["s"].get<bool>());
    else {
      std::cerr << "warning: 's' not boolean\n";
    }
  }
  return true;
}

int main(int argc, char** argv)
{
  // Initialize protobuf library (if needed)
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Prepare zenoh session and message collector
  MessageCollector collector;
  zenoh::Config cfg = zenoh::Config::create_default();
  auto z = zenoh::Session::open(std::move(cfg));
  // Create a subscriber for 'out/**'
  auto sub = z.declare_subscriber(
    zenoh::KeyExpr("output/**"),
    // on data
    [&collector](const zenoh::Sample& s) {
      // sample.payload is a Data object; convert to string
      std::string payload = s.get_payload().as_string();
      collector.push(payload);
    },
    zenoh::closures::none);

  // Create a publisher for 'input/'
  auto pub = z.declare_publisher("esmini/gt");

  // Vector to collect outputs (the "ArrayList")
  std::vector<std::string> results;

  // Recursively find .jsonl files
  for(auto it = fs::recursive_directory_iterator(test_path);
      it != fs::recursive_directory_iterator();
      ++it) {
    try {
      if(!it->is_regular_file())
        continue;
      fs::path p = it->path();
      if(p.extension() == ".jsonl") {
        std::string fullpath = p.string();
        std::string filename = p.filename().string();
        std::cout << "Found .jsonl file: " << fullpath << " (name: " << filename
                  << ")\n";

        // Look for same-name.yaml in same directory
        fs::path yaml_path = p.parent_path() / (p.stem().string() + ".yaml");
        if(fs::exists(yaml_path)) {
          auto patternOpt = read_pattern_from_yaml(yaml_path);
          if(patternOpt) {
            std::string transformed = transform_pattern(*patternOpt);
            std::cout << "Original pattern: " << *patternOpt << "\n";
            std::cout << "Transformed pattern: " << transformed << "\n";

            // Update settings.json (as requested)
            if(!update_settings_monitor_pattern(settings_path, transformed)) {
              std::cerr << "Failed updating settings file\n";
            }
            else {
              std::cout << "Updated settings file at " << settings_path << "\n";
            }
          }
          else {
            std::cerr << "Could not read pattern from YAML " << yaml_path
                      << "\n";
          }
        }
        else {
          std::cerr << "YAML file not found for " << filename
                    << " at expected path " << yaml_path << "\n";
        }

        // Now open the .jsonl and process line-by-line
        std::ifstream ifs(fullpath);
        if(!ifs.is_open()) {
          std::cerr << "Failed to open " << fullpath << "\n";
          continue;
        }

        // Ensure all zenoh_bridge processes are killed before starting a new
        // one
        while(
          std::system(
            "pkill -9 -f /zenoh-bridge/apps/zenoh_bridge/build/zenoh_bridge > "
            "/dev/null") == 0) {
          std::cerr << "Killed the non-terminated one." << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // Start zenoh_bridge as a separate process and wait 3 seconds for
        // initialization
        int ret = std::system((std::string(zenoh_bridge) + " &").c_str());
        if(ret == -1) {
          std::cerr << "Failed to start zenoh_bridge process\n";
        }
        else {
          std::cout << "Started zenoh_bridge, waiting for initialization...\n";
          std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        std::string line;
        while(std::getline(ifs, line)) {
          if(line.empty())
            continue;
          try {
            json obj = json::parse(line);
            LTLtest msg;
            json_to_protobuf(obj, msg);

            std::vector<uint8_t> midstep(msg.ByteSizeLong());
            if(!msg.SerializeToArray(midstep.data(), msg.ByteSizeLong())) {
              std::cerr << "Failed to serialize protobuf\n";
              continue;
            }
            pub.put(midstep);

            // Wait for a response on out/**
            auto responseOpt = collector.wait_pop(
              std::chrono::milliseconds(10000));  // 10s timeout
            if(responseOpt) {
              // push response payload into results list
              results.push_back(*responseOpt);
              //   std::cout << "Received response (len=" << responseOpt->size()
              //             << ") and appended to results\n";
            }
            else {
              std::cerr << "No response received within timeout after "
                           "publishing one message.\n";
            }
          }
          catch(const std::exception& e) {
            std::cerr << "Failed to parse line as JSON or process it: "
                      << e.what() << "\n";
            continue;
          }
        }  // end reading lines

        std::system("pkill -f zenoh_bridge");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ifs.close();
        if(!results.empty()) {
          double sum = 0.0;
          size_t count = 0;
          double sum2 = 0.0;
          size_t count2 = 0;
          for(const auto& res_str : results) {
            try {
              json res_json = json::parse(res_str);
              if(
                res_json.contains("update_mapper_us") &&
                res_json["update_mapper_us"].is_number()) {
                sum += res_json["update_mapper_us"].get<double>();
                ++count;
              }
              if(
                res_json.contains("update_monitor_us") &&
                res_json["update_monitor_us"].is_number()) {
                sum2 += res_json["update_monitor_us"].get<double>();
                ++count2;
              }
            }
            catch(const std::exception& e) {
              std::cerr << "Failed to parse result as JSON: " << e.what()
                        << "\n";
            }
          }
          if(count > 0) {
            double mean = sum / count;
            std::cout << "Mean update_mapper_us: " << mean << "\n";
          }
          else {
            std::cout << "No valid update_mapper_us values found in results.\n";
          }
          if(count2 > 0) {
            double mean2 = sum2 / count2;
            std::cout << "Mean update_monitor_us: " << mean2 << "\n";
          }
          results.clear();
        }
      }
    }
    catch(const std::exception& e) {
      std::cerr << "Error iterating filesystem: " << e.what() << "\n";
    }
  }

  // Optionally print collected results
  std::cout << "\nCollected " << results.size() << " response(s):\n";
  for(size_t i = 0; i < results.size(); ++i) {
    std::cout << i << ": (len=" << results[i].size() << ")\n";
  }

  // Shutdown protobuf (optional)
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
