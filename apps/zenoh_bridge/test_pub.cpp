#include <stdio.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "reelay/monitors.hpp"
#include "zenoh.hxx"

using namespace std::chrono_literals;

using time_type = int64_t;
using config_type = reelay::json;

int main(int argc, char *argv[]) {
    zenoh::Config test_pub_config = zenoh::Config::create_default();
    auto session = zenoh::Session::open(std::move(test_pub_config));

    auto pub = session.declare_publisher(zenoh::KeyExpr("keyexpr/test"));
    std::vector<uint8_t> buffer2 = {0x08, 0x00, 0x10, 0x64, 0x18, 0xF5, 0x2D};


    while (true) {
        printf("Sending data\n");
        std::this_thread::sleep_for(1s);
        // pub.put(R"({"test": true})");
        for (uint8_t byte : buffer2) {
          // static_cast<int> converts the byte to an integer to avoid printing a character
          std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
                    << static_cast<int>(byte) << " ";
      }
        pub.put(buffer2);
      }
}
