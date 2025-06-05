#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "zenoh.hxx"
#include "reelay/monitors.hpp"
#include <chrono>
#include <fstream>

using json = reelay::json;
using namespace std::chrono;

#define OSI_OUT_PORT 48198
#define MAX_MSG_SIZE 1024000
#define OSI_MAX_UDP_DATA_SIZE 8200
#define ES_SERV_TIMEOUT 500
#define ZENOH_TOPIC "esmini/gt"

static bool quit = false;

void CloseGracefully(int socket) {
    if (close(socket) < 0) {
        perror("Failed closing socket");
    }
}

void signal_handler(int) {
    quit = true;
}
auto now = steady_clock::now();

int main() {
    signal(SIGINT, signal_handler);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    struct timeval tv = {ES_SERV_TIMEOUT / 1000, (ES_SERV_TIMEOUT % 1000) * 1000};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(OSI_OUT_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
        perror("Bind failed");
        CloseGracefully(sock);
        return -1;
    }

    printf("Listening on port %d. Press Ctrl-C to quit.\n", OSI_OUT_PORT);

    zenoh::Config config = zenoh::Config::create_default();
    // config.insert_json5("connect/endpoints", R"(["tcp/127.0.0.1:7447"])");
    // config.insert_json5("listen/endpoints", R"(["tcp/0.0.0.0:7449"])");
    auto session = zenoh::Session::open(std::move(config));
    auto pub = session.declare_publisher(zenoh::KeyExpr(ZENOH_TOPIC));

    struct {
        int counter;
        unsigned int datasize;
        char data[OSI_MAX_UDP_DATA_SIZE];
    } buf;

    char large_buf[MAX_MSG_SIZE];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_size = sizeof(sender_addr);

    json json_object = json::array(); // JSON array to store send time and package number
    auto last_publish_time = steady_clock::now();
    std::vector<std::chrono::microseconds::rep> send_times;
    uint64_t total_receivedDataBytes = 0;
    uint64_t max_recieved = 0;
    std::vector<std::chrono::microseconds::rep> udp_intervals; // Store intervals between packets
    auto last_receive_time = steady_clock::now(); // Track the last receive time


    while (!quit) {
        buf.counter = 1;
        int receivedDataBytes = 0;

        while (buf.counter > 0) {
            int retval = recvfrom(sock, &buf, sizeof(buf), 0,
                                  reinterpret_cast<struct sockaddr*>(&sender_addr), &sender_addr_size);
            if (retval > 0) {
                if (buf.counter == 1) receivedDataBytes = 0;
                memcpy(&large_buf[receivedDataBytes], buf.data, buf.datasize);
                receivedDataBytes += buf.datasize;
                now = steady_clock::now();
                auto interval = duration_cast<microseconds>(now - last_receive_time).count();
                udp_intervals.push_back(interval);
                last_receive_time = now; // Update the last receive time
            }
            if (duration_cast<seconds>(steady_clock::now() - last_receive_time).count() >= 10) {
                for (int i = 0; i < send_times.size(); i++) {
                    json_object.push_back({{"package_number", i}, {"send_time", send_times[i]}});
                }
                std::cout << total_receivedDataBytes/send_times.size() << std::endl;
                std::cout << max_recieved << std::endl;
                std::ofstream file("/zenoh-bridge/sender.json");
                file << json_object.dump(4); // Save JSON object to file with indentation
                file.close();
                if (!udp_intervals.empty()) {
                    // Calculate the mean interval
                    auto total_intervals = std::accumulate(udp_intervals.begin(), udp_intervals.end(), 0LL);
                    auto mean_interval = total_intervals / udp_intervals.size();
                    std::cout << "Mean UDP Interval: " << mean_interval << " µs" << std::endl;
                } else {
                    std::cout << "No UDP packets received." << std::endl;
                }
                break; // Exit the loop to end the program
            }
        }

        // if (receivedDataBytes > 0) {
        //     pub.put(std::string(large_buf, receivedDataBytes));
        //     total_receivedDataBytes += receivedDataBytes;
        //     if (receivedDataBytes > max_recieved) {
        //         max_recieved = receivedDataBytes;
        //     }

        //     // Record send time and package number
        //     now = steady_clock::now();
        //     auto send_time = duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        //     send_times.push_back(send_time);
        //     // package_number++;

        //     last_publish_time = steady_clock::now(); // Update last publish time
        // } else {
        //     usleep(500); // Sleep for 10ms
        // }

        // // Check if 10 seconds have passed since the last publish

    }
    for (int i = 0; i < send_times.size(); i++) {
        json_object.push_back({{"package_number", i}, {"send_time", send_times[i]}});
    }
    std::cout << total_receivedDataBytes/send_times.size() << std::endl;
    std::cout << max_recieved << std::endl;
    std::ofstream file("/zenoh-bridge/sender.json");
    file << json_object.dump(4); // Save JSON object to file with indentation
    file.close();
    if (!udp_intervals.empty()) {
        // Calculate the mean interval
        auto total_intervals = std::accumulate(udp_intervals.begin(), udp_intervals.end(), 0LL);
        auto mean_interval = total_intervals / udp_intervals.size();
        std::cout << "Mean UDP Interval: " << mean_interval << " µs" << std::endl;
    } else {
        std::cout << "No UDP packets received." << std::endl;
    }
    

    CloseGracefully(sock);
    return 0;
}