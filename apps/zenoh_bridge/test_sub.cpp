#include "reelay/monitors.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "zenoh.hxx"

using namespace std::chrono_literals;
using json = reelay::json;

std::chrono::steady_clock::time_point last_input_time;
uint64_t total_time = 0;
int how_much = 0;
json json_data = json::array(); // JSON array to store receive time and package number

void data_handler2(const zenoh::Sample& sample)
{
    auto now = std::chrono::high_resolution_clock::now();
    last_input_time = std::chrono::steady_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                         now.time_since_epoch())
                         .count();
    how_much++;

    // Record receive time and package number
    json_data.push_back({{"package_number", how_much}, {"receive_time", timestamp}});

    std::cout << sample.get_payload().as_string() << std::endl;
}

int main(int argc, char* argv[])
{
    zenoh::Config sub_config_def = zenoh::Config::create_default();
    auto sub_session = zenoh::Session::open(std::move(sub_config_def));
    auto subscriber = sub_session.declare_subscriber(
        zenoh::KeyExpr("output/**"), &data_handler2, zenoh::closures::none);

    last_input_time = std::chrono::steady_clock::now(); // Initialize last input time

    std::thread timeout_thread([&]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_input_time).count();
            if (elapsed_time > 10) {
                std::cout << "No input for 10 seconds, saving JSON and exiting..." << std::endl;

                // Save JSON object to file
                std::ofstream file("receiver.json");
                file << json_data.dump(4); // Save JSON object with indentation
                file.close();
            }
        }
    });

    timeout_thread.join(); // Wait for the timeout thread to finish
    return 0;
}