#include "reelay/monitors.hpp"
#include "zenoh.hxx"
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <thread>

#include "osi_common.pb.h"
#include "osi_object.pb.h"
#include "osi_sensorview.pb.h"
#include "osi_version.pb.h"

#include <google/protobuf/util/json_util.h>
using namespace std::chrono_literals;

using time_type = int64_t;
using input_type = reelay::json;
using output_type = reelay::json;
using config_type = reelay::json;

void data_handler(const zenoh::Sample &sample);
bool generate_monitor_node(std::string pub_keyexpr, config_type pub_config,
                           std::string sub_keyexpr, config_type sub_config,
                           config_type monitor_config);

int main() {
    config_type pub_config = {};
    config_type sub_config = {};
    config_type monitor_config = reelay::json::parse(R"( {
    "monitor_type": "discrete",
    "pattern": "historically[0:5]{timestamp != 80}"
    })");
    generate_monitor_node("output/1", pub_config, "esmini/gt", sub_config,
                          monitor_config);
}

reelay::monitor<input_type, output_type> *generated_monitor;
zenoh::Publisher *publisher_pnt;

// key_expr: keyexpr/test
// express:0
// payload:{"test": true}
// 0
// bruh2
// bruh3
// true
void data_handler(const zenoh::Sample &sample) {
    // std:: cout << sample.get_payload().as_string() << std::endl;
    osi3::GroundTruth gt;
    gt.ParseFromArray(sample.get_payload().as_string().c_str(),
                      sample.get_payload().size());
    std::string json_string;
    google::protobuf::util::MessageToJsonString(gt, &json_string);
    std::cout << json_string << std::endl;
    // input_type message = reelay::json::parse(json_string);
    // auto result = generated_monitor->update(message);
    // std::cout << result["value"] << std::endl;
    // publisher_pnt->put(result.dump());
}

inline bool generate_monitor_node(std::string pub_keyexpr,
                                  config_type pub_config,
                                  std::string sub_keyexpr,
                                  config_type sub_config,
                                  config_type monitor_config) {

    // Initialize the subscriber node
    zenoh::Config sub_config_def = zenoh::Config::create_default();
    for (auto &option : sub_config.items()) {
        sub_config_def.insert_json5(option.key(), option.value().dump());
    }
    auto sub_session = zenoh::Session::open(std::move(sub_config_def));
    auto subscriber = sub_session.declare_subscriber(
        zenoh::KeyExpr(sub_keyexpr), &data_handler, zenoh::closures::none);

    // Initialize the monitor
    if (monitor_config["monitor_type"] == "discrete") {
        auto options =
            reelay::discrete_timed<time_type>::monitor<input_type,
                                                       output_type>::options()
                .disable_condensing();
        generated_monitor = new reelay::monitor<input_type, output_type>(
            reelay::make_monitor(monitor_config["pattern"], options));
    } else if (monitor_config["monitor_type"] == "dense") {
        auto options =
            reelay::dense_timed<double>::monitor<input_type,
                                                 output_type>::options();
        generated_monitor = new reelay::monitor<input_type, output_type>(
            reelay::make_monitor(monitor_config["pattern"], options));
    } else {
        return false;
    }

    // Initialize the publisher node
    zenoh::Config pub_config_def = zenoh::Config::create_default();
    for (auto &option : pub_config.items()) {
        pub_config_def.insert_json5(option.key(), option.value().dump());
    }

    // Ini
    auto pub_session = zenoh::Session::open(std::move(pub_config_def));
    auto publisher = pub_session.declare_publisher(zenoh::KeyExpr(pub_keyexpr));
    publisher_pnt = &publisher;
    while (true) {
        std::this_thread::sleep_for(1s);
    }
    return true;
}

// #include "zenoh.hxx"
// #include <google/protobuf/util/json_util.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include "osi_common.pb.h"
// #include "osi_object.pb.h"
// #include "osi_sensorview.pb.h"
// #include "osi_version.pb.h"
// #include "reelay/monitors/dense_timed_data_monitor.hpp"
// #include "reelay/monitors/monitor.hpp"

// #include "rvstd/resource.hpp"
// #include "rvstd/tableau.hpp"
// #include "rvstd/unique_identifier.hpp"
// #include <signal.h>
// using namespace google::protobuf::util;
// using namespace rvstd;
// using namespace std::chrono_literals;
// using time_type = int64_t;
// using input_type = reelay::json;
// using output_type = reelay::json;
// using config_type = reelay::json;

// osi3::GroundTruth gt;
// std::unordered_map <std::string, reelay::monitor<input_type, output_type>* >
// monitors;

// void data_handler(const zenoh::Sample &sample) {
//     auto monitor = monitors[sample.get_keyexpr().as_string_view().data()];
//     monito
// }
// int main() {

//     // Define your patterns and options
//     std::string pattern1 = "pattern1_definition";
//     std::string pattern2 = "pattern2_definition";
//     reelay::basic_options options;
// ""
//     // Instantiate a shared data manager
//     auto data_manager = std::make_shared<reelay::binding_manager>();
//     options.with_data_manager(data_manager);

//     // Create two monitors with the same data manager
//     auto monitor1 =
//         reelay::dense_timed_data_monitor<time_type, input_type,
//                                          output_type>::make_shared(pattern1,
//                                                                    options);
//     auto monitor2 =
//         reelay::dense_timed_data_monitor<time_type, input_type,
//                                          output_type>::make_shared(pattern2,
//                                                                    options);

//     // Ensure both monitors use the same data manager
// }

/*
 * esmini - Environment Simulator Minimalistic
 * https://github.com/esmini/esmini
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) partners of Simulation Scenarios
 * https://sites.google.com/view/simulationscenarios
 */

// #ifndef _WINDOWS
// #include <unistd.h>
// #define Sleep(x) usleep((x)*1000)
// #endif

// static bool quit;

// #ifdef _WIN32
// #include <winsock2.h>
// #include <Ws2tcpip.h>
// typedef SOCKET SE_SOCKET;
// #define SE_INVALID_SOCKET INVALID_SOCKET
// #else
// /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
// #include <unistd.h> /* Needed for close() */
// typedef int SE_SOCKET;
// #define SE_INVALID_SOCKET -1
// #endif

// #define OSI_OUT_PORT          48198
// #define ES_SERV_TIMEOUT       500
// #define MAX_MSG_SIZE          1024000
// #define OSI_MAX_UDP_DATA_SIZE 8200

// void CloseGracefully(SE_SOCKET socket)
// {
// #ifdef _WIN32
//     if (closesocket(socket) == SOCKET_ERROR)
// #else
//     if (close(socket) < 0)
// #endif
//     {
//         printf("Failed closing socket");
//     }

// #ifdef _WIN32
//     WSACleanup();
// #endif
// }

// static void signal_handler(int s)
// {
//     printf("Caught signal %d - quit\n", s);

//     quit = true;
// }

// int main(int argc, char* argv[])
// {
//     (void)argc;
//     (void)argv;
//     static SE_SOCKET          sock;
//     struct sockaddr_in        server_addr;
//     struct sockaddr_in        sender_addr;
//     static unsigned short int iPortIn = OSI_OUT_PORT;  // Port for incoming
//     packages static char               large_buf[MAX_MSG_SIZE]; socklen_t
//     sender_addr_size = sizeof(sender_addr);

//     // This struct must match the sender side
//     struct
//     {
//         int          counter;
//         unsigned int datasize;
//         char         data[OSI_MAX_UDP_DATA_SIZE];
//     } buf;

//     quit = false;

//     // Setup signal handler to catch Ctrl-C
//     signal(SIGINT, signal_handler);

// #ifdef _WIN32
//     WSADATA wsa_data;
//     int     iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
//     if (iResult != NO_ERROR)
//     {
//         wprintf(L"WSAStartup failed with error %d\n", iResult);
//         return -1;
//     }
// #endif

//     sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//     if (sock == SE_INVALID_SOCKET)
//     {
//         printf("socket failed\n");
//         return -1;
//     }

//     // set timer for receive operations
// #ifdef _WIN32
//     int timeout_msec = ES_SERV_TIMEOUT;
//     if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_msec,
//     sizeof(timeout_msec)) < 0)
// #else
//     struct timeval tv;
//     tv.tv_sec  = ES_SERV_TIMEOUT / 1000;
//     tv.tv_usec = (ES_SERV_TIMEOUT % 1000) * 1000;
//     if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0)
// #endif
//     {
//         printf("socket SO_RCVTIMEO (receive timeout) not supported on this
//         platform\n");
//     }

//     server_addr.sin_family      = AF_INET;
//     server_addr.sin_port        = htons(iPortIn);
//     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

//     if (bind(sock, reinterpret_cast<struct sockaddr*>(&server_addr),
//     sizeof(server_addr)) != 0)
//     {
//         printf("Bind failed");
//         CloseGracefully(sock);
//         return -1;
//     }

//     printf("Socket open. Waiting for OSI messages on port %d. Press Ctrl-C to
//     quit.\n", OSI_OUT_PORT);

//     osi3::GroundTruth gt;

//     while (!quit)
//     {
//         // Fetch and parse OSI message
//         buf.counter = 1;
//         int retval;
//         int receivedDataBytes = 0;
//         while (buf.counter > 0)
//         {
//             retval = static_cast<int>(
//                 recvfrom(sock, reinterpret_cast<char*>(&buf), sizeof(buf), 0,
//                 reinterpret_cast<struct sockaddr*>(&sender_addr),
//                 &sender_addr_size));
//             if (retval > 0)
//             {
//                 if (buf.counter == 1)
//                 {
//                     // New message
//                     receivedDataBytes = 0;
//                 }
//                 memcpy(&large_buf[receivedDataBytes], buf.data,
//                 buf.datasize); receivedDataBytes +=
//                 static_cast<int>(buf.datasize);
//             }
//         }

//         if (retval > 0)
//         {
//             gt.ParseFromArray(large_buf, receivedDataBytes);

//             // Print timestamp
//             printf("timestamp: %.2f\n",
//                    static_cast<double>(gt.mutable_timestamp()->seconds()) +
//                    1E-9 *
//                    static_cast<double>(gt.mutable_timestamp()->nanos()));

//             // Print object id, position, orientation and velocity
//             for (int i = 0; i < gt.mutable_moving_object()->size(); i++)
//             {
//                 printf(" obj id %d pos (%.2f, %.2f, %.2f) orientation (%.2f,
//                 %.2f, %.2f) velocity (%.2f, %.2f, %.2f) \n",
//                        static_cast<int>(gt.mutable_moving_object(i)->mutable_id()->value()),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_position()->x(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_position()->y(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_position()->z(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_orientation()->yaw(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_orientation()->pitch(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_orientation()->roll(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_velocity()->x(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_velocity()->y(),
//                        gt.mutable_moving_object(i)->mutable_base()->mutable_velocity()->z());
//             }
//         }
//         else
//         {
//             // No incoming messages, wait for a little while before polling
//             again Sleep(10);
//         }
//     }

//     return 0;
// }
