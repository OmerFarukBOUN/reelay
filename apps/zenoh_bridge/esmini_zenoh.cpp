// #include "zenoh.hxx"
// #include <boost/asio.hpp>
// #include <fstream>
// #include <google/protobuf/util/json_util.h> // To convert protobuf to JSON
// #include <iostream>
// #include <string>
// #include <thread>

// using boost::asio::ip::tcp;

// zenoh::Publisher *publisher_pnt;

// void handle_client(tcp::socket socket) {
//     try {
//         // Read incoming data from socket
//         std::vector<char> buffer(1024);
//         size_t len = socket.read_some(boost::asio::buffer(buffer));
//         publisher_pnt->put(std::string(buffer.data(), len));
//         std::cout << "Data published." << std::endl;

//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
// }

// int main() {
//     try {
//         // Set up server to listen on port 48198
//         boost::asio::io_service io_service;
//         tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 48198));

//         std::cout << "Server listening on port 48198..." << std::endl;
//         zenoh::Config test_pub_config = zenoh::Config::create_default();
//         auto session = zenoh::Session::open(std::move(test_pub_config));

//         auto pub = session.declare_publisher(zenoh::KeyExpr("esmini/gt"));
//         publisher_pnt = &pub;

//         while (true) {
//             // Accept incoming client connection
//             tcp::socket socket(io_service);
//             acceptor.accept(socket);
//             std::cout << "Connection established." << std::endl;

//             // Handle client in a separate thread
//             std::thread(handle_client, std::move(socket)).detach();
//         }
//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }

//     return 0;
// }

// #include <iostream>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include "zenoh.hxx"
// #include <boost/asio.hpp>
// #include <fstream>
// #include <google/protobuf/util/json_util.h> // To convert protobuf to JSON
// #include <string>
// #include <thread>

// #define PORT 48198
// #define MAX_BUFFER_SIZE 65507 // Maximum UDP packet size (64KB)

// zenoh::Publisher *publisher_pnt;

// int main() {
//     std::cout << "Server listening on port 48198..." << std::endl;
//     zenoh::Config test_pub_config = zenoh::Config::create_default();
//     auto session = zenoh::Session::open(std::move(test_pub_config));

//     auto pub = session.declare_publisher(zenoh::KeyExpr("esmini/gt"));
//     publisher_pnt = &pub;
//     // Create UDP socket
//     int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (sockfd < 0) {
//         std::cerr << "Error: Unable to create socket\n";
//         return 1;
//     }

//     // Set up the server address structure
//     struct sockaddr_in server_addr, client_addr;
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(PORT);

//     // Bind the socket to the address and port
//     if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
//     0) {
//         std::cerr << "Error: Unable to bind socket\n";
//         close(sockfd);
//         return 1;
//     }

//     std::cout << "Listening for UDP packets on port " << PORT << "...\n";

//     // Define buffer with maximum size for UDP packets
//     char buffer[MAX_BUFFER_SIZE];
//     socklen_t addr_len = sizeof(client_addr);

//     // Receive and print UDP packets
//     while (true) {
//         ssize_t recv_len = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0,
//         (struct sockaddr*)&client_addr, &addr_len); if (recv_len < 0) {
//             std::cerr << "Error: Unable to receive data\n";
//             break;
//         }
//         publisher_pnt->put(std::string(buffer, recv_len));
//         std::cout << "Data published." << std::endl;
//     }

//     // Close the socket before exiting
//     close(sockfd);
//     return 0;
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

#include "zenoh.hxx"
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osi_common.pb.h"
#include "osi_object.pb.h"
#include "osi_sensorview.pb.h"
#include "osi_version.pb.h"
#include <signal.h>

#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)

static bool quit;

/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <arpa/inet.h>
#include <netdb.h> /* Needed for getaddrinfo() and freeaddrinfo() */
#include <sys/socket.h>
#include <unistd.h> /* Needed for close() */
typedef int SE_SOCKET;
#define SE_INVALID_SOCKET -1

#define OSI_OUT_PORT 48198
#define ES_SERV_TIMEOUT 500
#define MAX_MSG_SIZE 1024000
#define OSI_MAX_UDP_DATA_SIZE 8200

void CloseGracefully(SE_SOCKET socket) {
    if (close(socket) < 0) {
        printf("Failed closing socket");
    }
}

static void signal_handler(int s) {
    printf("Caught signal %d - quit\n", s);

    quit = true;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    static SE_SOCKET sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in sender_addr;
    static unsigned short int iPortIn =
        OSI_OUT_PORT; // Port for incoming packages
    static char large_buf[MAX_MSG_SIZE];
    socklen_t sender_addr_size = sizeof(sender_addr);

    // This struct must match the sender side
    struct {
        int counter;
        unsigned int datasize;
        char data[OSI_MAX_UDP_DATA_SIZE];
    } buf;

    quit = false;

    // Setup signal handler to catch Ctrl-C
    signal(SIGINT, signal_handler);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == SE_INVALID_SOCKET) {
        printf("socket failed\n");
        return -1;
    }

    // set timer for receive operations

    struct timeval tv;
    tv.tv_sec = ES_SERV_TIMEOUT / 1000;
    tv.tv_usec = (ES_SERV_TIMEOUT % 1000) * 1000;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0) {
        printf("socket SO_RCVTIMEO (receive timeout) not supported on this "
               "platform\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(iPortIn);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, reinterpret_cast<struct sockaddr *>(&server_addr),
             sizeof(server_addr)) != 0) {
        printf("Bind failed");
        CloseGracefully(sock);
        return -1;
    }

    printf("Socket open. Waiting for OSI messages on port %d. Press Ctrl-C to "
           "quit.\n",
           OSI_OUT_PORT);

    osi3::GroundTruth gt;
    std::cout << "Server listening on port 48198..." << std::endl;
    zenoh::Config test_pub_config = zenoh::Config::create_default();
    auto session = zenoh::Session::open(std::move(test_pub_config));

    auto pub = session.declare_publisher(zenoh::KeyExpr("esmini/gt"));

    while (!quit) {
        // Fetch and parse OSI message
        buf.counter = 1;
        int retval;
        int receivedDataBytes = 0;
        while (buf.counter > 0) {
            retval = static_cast<int>(
                recvfrom(sock, reinterpret_cast<char *>(&buf), sizeof(buf), 0,
                         reinterpret_cast<struct sockaddr *>(&sender_addr),
                         &sender_addr_size));
            if (retval > 0) {
                if (buf.counter == 1) {
                    // New message
                    receivedDataBytes = 0;
                }
                memcpy(&large_buf[receivedDataBytes], buf.data, buf.datasize);
                receivedDataBytes += static_cast<int>(buf.datasize);
            }
        }

        if (retval > 0) {
            osi3::GroundTruth gt;
            gt.ParseFromArray(large_buf, receivedDataBytes);
            // std::cout << gt.DebugString() << std::endl;

            pub.put(std::string(large_buf, receivedDataBytes));
            // std::cout << large_buf << std::endl;

        } else {
            // No incoming messages, wait for a little while before polling
            // again
            Sleep(10);
        }
    }

    return 0;
}
