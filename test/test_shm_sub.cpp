#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <condition_variable>
#include "zenoh.hxx"

using namespace zenoh;
using namespace std::chrono;

std::mutex mtx;
std::condition_variable cv;
bool message_received = false;

struct Result {
    std::vector<double> latencies;
    size_t expected_size = 0;
    int count = 0;
};

std::map<size_t, Result> results;

void data_handler(Sample &sample) {
    auto recv_time = steady_clock::now();
    const auto &payload = sample.get_payload();
    auto bytes = payload.as_vector();

    if (bytes.size() < sizeof(uint64_t)) return;

    uint64_t send_ns;
    memcpy(&send_ns, bytes.data(), sizeof(uint64_t));
    auto send_time = steady_clock::time_point(nanoseconds(send_ns));
    double latency_us = duration_cast<duration<double, std::micro>>(recv_time - send_time).count();

    size_t total_size = bytes.size();
    {
        std::lock_guard<std::mutex> lock(mtx);
        results[total_size].latencies.push_back(latency_us);
        results[total_size].count++;
        message_received = true;
    }
    cv.notify_one();
}

int main() {
#ifdef ZENOHCXX_ZENOHC
    init_log_from_env_or("error");
#endif

    std::cout << "[Subscriber] Opening session...\n";
    auto session = Session::open(Config::create_default());
    KeyExpr keyexpr("demo/latency");
    auto sub = session.declare_subscriber(keyexpr, &data_handler, closures::none);
    std::cout << "[Subscriber] Ready.\n";

    // Wait until 6 * 1000 messages received
    int total_expected = 6 * 1000;
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return message_received; });
        message_received = false;

        int total = 0;
        for (auto &p : results) total += p.second.count;
        if (total >= total_expected) break;
    }

    std::cout << "\n=== Latency Results (Average per Size) ===\n";
    for (auto &[sz, r] : results) {
        double avg = 0;
        for (auto v : r.latencies) avg += v;
        avg /= r.latencies.size();
        std::cout << sz << " bytes -> " << avg << " µs (n=" << r.latencies.size() << ")\n";
    }

    std::cout << "[Subscriber] Done.\n";
    return 0;
}
