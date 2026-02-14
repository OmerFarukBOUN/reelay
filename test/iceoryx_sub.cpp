#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include <chrono>
#include <iostream>
#include <thread>

constexpr char APP_NAME[] = "latency-subscriber";
constexpr const char* SERVICE = "Latency";
constexpr const char* INSTANCE = "Test";

template<size_t N>
struct Payload {
    uint8_t data[N];
    uint64_t timestamp_ns;
};

template<size_t N>
void receive_samples(const char* event, iox::popo::Subscriber<Payload<N>>& sub, int num_samples)
{
    std::cout << "Receiving " << num_samples << " samples of " << event << "..." << std::endl;
    uint64_t total_ns = 0;
    int count = 0;

    while (count < num_samples)
    {
        sub.take()
            .and_then([&](auto& sample) {
                auto now = std::chrono::steady_clock::now().time_since_epoch();
                uint64_t recv_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
                total_ns += (recv_ns - sample->timestamp_ns);
                ++count;
            })
            .or_else([](auto&) {});
    }

    double avg_us = static_cast<double>(total_ns) / num_samples / 1000.0;
    std::cout << "Average latency for " << event << ": " << avg_us << " µs" << std::endl;
}

int main()
{
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    constexpr int num_samples = 1000;

    iox::popo::Subscriber<Payload<10>>   sub10({"Latency", "Test", "10B"});
    iox::popo::Subscriber<Payload<100>>  sub100({"Latency", "Test", "100B"});
    iox::popo::Subscriber<Payload<1024>> sub1K({"Latency", "Test", "1KB"});
    iox::popo::Subscriber<Payload<2048>> sub2K({"Latency", "Test", "2KB"});
    iox::popo::Subscriber<Payload<4096>> sub4K({"Latency", "Test", "4KB"});
    iox::popo::Subscriber<Payload<1000000>> sub8K({"Latency", "Test", "8KB"});

    receive_samples("10B",  sub10,  num_samples);
    receive_samples("100B", sub100, num_samples);
    receive_samples("1KB",  sub1K,  num_samples);
    receive_samples("2KB",  sub2K,  num_samples);
    receive_samples("4KB",  sub4K,  num_samples);
    receive_samples("8KB",  sub8K,  num_samples);

    std::cout << "All measurements done.\n";
    return 0;
}
