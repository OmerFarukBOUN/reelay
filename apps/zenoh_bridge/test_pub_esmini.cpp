#include <stdio.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "osi_hostvehicledata.pb.h"
#include "zenoh.hxx"

using namespace std::chrono_literals;

using time_type = int64_t;

int main(int argc, char *argv[]) {
    zenoh::Config test_pub_config = zenoh::Config::create_default();
    test_pub_config.insert_json5("connect/endpoints", R"(["tcp/127.0.0.1:7447"])");
    test_pub_config.insert_json5("listen/endpoints", R"(["tcp/0.0.0.0:7447"])");
    
    zenoh::try_init_log_from_env();
    auto session = zenoh::Session::open(std::move(test_pub_config));

    auto pub = session.declare_publisher(zenoh::KeyExpr("esmini/in/0"));
    osi3::HostVehicleData data;
    data.mutable_vehicle_powertrain()->set_pedal_position_acceleration(1.0);
    data.mutable_vehicle_brake_system()->set_pedal_position_brake(0);
    data.mutable_vehicle_steering()->mutable_vehicle_steering_wheel()->set_angle(0);


    while (true) {
        // printf("Sending data\n");
        std::this_thread::sleep_for(200ms);
    //     // pub.put(R"({"test": true})");
    //     for (uint8_t byte : buffer2) {
    //       // static_cast<int> converts the byte to an integer to avoid printing a character
    //       std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
    //                 << static_cast<int>(byte) << " ";
    //   }
        pub.put(data.SerializeAsString());
        std::cout << "bruh" << std::endl;
      }
}
