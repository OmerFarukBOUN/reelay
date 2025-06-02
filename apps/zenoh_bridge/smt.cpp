// main.cpp

#include <iostream>
#include <vector>
#include <cstring>
#include <system_error>
#include <csignal>
#include <fstream>
#include <atomic>

// POSIX sockets:
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "reelay/monitors.hpp"
using json = reelay::json;
using namespace std::chrono;

// Zenoh C++ API:
#include "zenoh.hxx"

using namespace zenoh;

// We’ll use an atomic flag to allow clean shutdown on SIGINT (Ctrl+C).
static std::atomic<bool> keep_running{true};

void sigint_handler(int)
{
    keep_running = false;
}

int main(int argc, char** argv)
{
    // -------------------------------
    // 1) REGISTER SIGINT HANDLER
    // -------------------------------
    std::signal(SIGINT, sigint_handler);

    // -------------------------------
    // 2) INITIALIZE ZENOH SESSION
    // -------------------------------
    //
    // Create a default Zenoh configuration and open a session.  By default
    // this will attempt to discover a Zenoh router on the network (UDP multicast
    // plus default TCP ports).  If you need to force a specific locator,
    // you can modify `config` accordingly (e.g. add .routers() or .mode()).
    //
    Config config = Config::create_default();
    auto session = zenoh::Session::open(std::move(config));

    // Declare a publisher on "esmini/gt".  All incoming packets will be forwarded here.
    Publisher publisher = session.declare_publisher(KeyExpr("esmini/gt"));

    std::cout << "[Info] Zenoh publisher declared on \"esmini/gt\". "
              << "Listening for UDP on port 48198...\n";

    // -------------------------------
    // 3) SET UP UDP SOCKET (PORT 48198)
    // -------------------------------
    //
    // We create a non‐blocking UDP socket bound to port 48198 on all interfaces.
    // We also bump up the OS receive buffer to avoid packet drops if bursts arrive.
    //
    int sockfd = ::socket(AF_INET, SOCK_DGRAM, /*protocol=*/0);
    if (sockfd < 0) {
        std::perror("[Error] socket()");
        return 1;
    }

    // Allow immediate reuse of the port if the program restarts:
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        std::perror("[Warning] setsockopt(SO_REUSEADDR)");
        // Not fatal—just continue.
    }

    // Increase the kernel receive buffer to 4 MB (default is often ~212 KB).
    // This helps absorb bursts of incoming packets without dropping.
    int rcvbuf_size = 4 * 1024 * 1024;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) < 0) {
        std::perror("[Warning] setsockopt(SO_RCVBUF)");
        // Not fatal; we’ll continue with default buffer size.
    }

    // Bind to port 48198 on all interfaces (0.0.0.0).
    sockaddr_in servaddr;
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(48198);

    if (::bind(sockfd, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr)) < 0) {
        std::perror("[Error] bind()");
        ::close(sockfd);
        return 1;
    }

    // Use a pre‐allocated buffer of 8 192 bytes (enough for packets up to ~8 KB).
    // Packets of length ~7027 bytes (as mentioned) will fit just fine.
    std::vector<uint8_t> buffer;
    buffer.resize(8192);

    // -------------------------------
    // 4) RECEIVE & FORWARD LOOP
    // -------------------------------
    //
    // We do a blocking recvfrom (waiting indefinitely for each UDP datagram).
    // As soon as we get bytes, we publish them to Zenoh synchronously.  If you
    // require more throughput, you could experiment with non‐blocking sockets +
    // an epoll/select loop or thread pools, but for many cases a single thread
    // with a tuned socket buffer is enough.
    //
    auto json_object = json::array(); // JSON array to store send time and package number
    auto last_publish_time = steady_clock::now();
    std::vector<std::chrono::microseconds::rep> send_times;
    uint64_t total_receivedDataBytes = 0;
    uint64_t max_recieved = 0;
    int count = 0;
    while (keep_running.load()) {
        sockaddr_in srcaddr;
        socklen_t addrlen = sizeof(srcaddr);

        // Blocking receive.  Returns the number of bytes read, or -1 on error.
        ssize_t n = ::recvfrom(sockfd,
                               buffer.data(),
                               static_cast<socklen_t>(buffer.size()),
                               0,
                               reinterpret_cast<sockaddr*>(&srcaddr),
                               &addrlen);

        if (n < 0) {
            // If interrupted by a signal (i.e. SIGINT), break out and clean up.
            if (errno == EINTR && !keep_running.load()) {
                break;
            }
            std::perror("[Warning] recvfrom()");
            continue;
        }

        // n is the number of bytes received (e.g. 7027).
        // Construct a Zenoh Bytes object (i.e. a std::vector<uint8_t>) from these bytes.
        //
        // We could move the existing buffer, but because the buffer is
        // pre-allocated, we copy only the exact `n` bytes into a new Bytes container
        // so that Zenoh does not hold onto the entire 8 192‐byte vector.
        //
        Bytes payload(std::vector<uint8_t>(buffer.begin(), buffer.begin() + n));

        // Publish to "esmini/gt".  This call is synchronous; if you want to
        // pipeline or batch multiple packets, you could queue them in a lock‐free
        // queue and publish on another thread, but for most setups this is already
        // quite fast.

        publisher.put(std::move(payload));
            total_receivedDataBytes += n;
            if (n > max_recieved) {
                max_recieved = n;
            }

            // Record send time and package number
            auto now = steady_clock::now();
            auto send_time = duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
            send_times.push_back(send_time);
            count ++;
            if (count ==667) {
                        for (int i = 0; i < send_times.size(); i++) {
                json_object.push_back({{"package_number", i}, {"send_time", send_times[i]}});
            }
            std::cout << total_receivedDataBytes/send_times.size() << std::endl;
            std::cout << max_recieved << std::endl;
            std::ofstream file("/zenoh-bridge/sender.json");
            file << json_object.dump(4); // Save JSON object to file with indentation
            file.close();
            break; // Exit the loop to end the program
            }
            

    }

    // -------------------------------
    // 5) CLEAN UP
    // -------------------------------
    std::cout << "\n[Info] Shutting down..." << std::endl;

    ::close(sockfd);
    session.close();           // close the Zenoh session

    std::cout << "[Info] Done." << std::endl;
    return 0;
}
