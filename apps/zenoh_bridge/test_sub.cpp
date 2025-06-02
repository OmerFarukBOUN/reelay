#include "reelay/monitors.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "zenoh.hxx"

using namespace std::chrono_literals;
using json = reelay::json;

std::chrono::steady_clock::time_point last_input_time;
std::vector<std::chrono::microseconds::rep> receive_times;
// JSON array to store receive time and package number

void data_handler2(const zenoh::Sample& sample)
{
    auto now = std::chrono::high_resolution_clock::now();
    last_input_time = std::chrono::steady_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                         now.time_since_epoch())
                         .count();

    receive_times.push_back(timestamp);

    // std::cout << sample.get_payload().as_string() << std::endl;
}

int main(int argc, char* argv[])
{
    zenoh::Config sub_config_def = zenoh::Config::create_default();
    // sub_config_def.insert_json5("connect/endpoints", R"(["tcp/127.0.0.1:7447"])"); // Connect to zenoh_bridge
    // sub_config_def.insert_json5("listen/endpoints", R"(["tcp/0.0.0.0:7448"])");       // Listen on its own port
    auto sub_session = zenoh::Session::open(std::move(sub_config_def));
    std::cout << "Declaring Subscriber on 'output/**'..." << std::endl;
    auto subscriber = sub_session.declare_subscriber(
        zenoh::KeyExpr("output/**"), &data_handler2, zenoh::closures::none);

    last_input_time = std::chrono::steady_clock::now(); // Initialize last input time


    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_input_time).count();
        if (elapsed_time > 10) {
            std::cout << "No input for 10 seconds, saving JSON and exiting..." << std::endl;
            json json_data = json::array(); 

            for (int i = 0; i < receive_times.size(); i++) {
                json_data.push_back({{"package_number", i}, {"recieve_time", receive_times[i]}});
            }

            // Save JSON object to file
            std::ofstream file("/zenoh-bridge/receiver.json");
            file << json_data.dump(4); // Save JSON object with indentation
            file.close();
            break;
            // exit(0); // Exit the program
        }
    }
        

    timeout_thread.join(); // Wait for the timeout thread to finish
    return 0;
}