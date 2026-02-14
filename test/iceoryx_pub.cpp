#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_posh/popo/publisher.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <cstring>
#include <vector>

constexpr char APP_NAME[] = "latency-publisher";
constexpr const char* SERVICE = "Latency";
constexpr const char* INSTANCE = "Test";

// Define payloads of different sizes
template<size_t N>
struct Payload {
    uint8_t data[N];
    uint64_t timestamp_ns;
};

template<size_t N>
void send_samples(const char* event, iox::popo::Publisher<Payload<N>>& pub, int num_samples)
{
    for (int i = 0; i < num_samples; ++i)
    {
        pub.loan().and_then([&](auto& sample) {
            std::memset(sample->data, 0xAB, N);
            sample->timestamp_ns =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count();
            sample.publish();
        }).or_else([](auto&) {
            std::cerr << "Loan failed\n";
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // prevent overlap
    }
}

int main()
{
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    constexpr int num_samples = 1000;

    iox::popo::Publisher<Payload<10>>   pub10({"Latency", "Test", "10B"});
    iox::popo::Publisher<Payload<100>>  pub100({"Latency", "Test", "100B"});
    iox::popo::Publisher<Payload<1024>> pub1K({"Latency", "Test", "1KB"});
    iox::popo::Publisher<Payload<2048>> pub2K({"Latency", "Test", "2KB"});
    iox::popo::Publisher<Payload<4096>> pub4K({"Latency", "Test", "4KB"});
    iox::popo::Publisher<Payload<1000000>> pub8K({"Latency", "Test", "8KB"});

    std::cout << "Sending " << num_samples << " samples for each size...\n";

    send_samples("10B",  pub10,  num_samples);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_samples("100B", pub100, num_samples);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_samples("1KB",  pub1K,   num_samples);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_samples("2KB",  pub2K,   num_samples);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_samples("4KB",  pub4K,   num_samples);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    send_samples("8KB",  pub8K,   num_samples);

    std::cout << "All data sent.\n";
    return 0;
}
