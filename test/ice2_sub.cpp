#include "iox2/iceoryx2.hpp"
#include <iostream>
#include <chrono>
#include <vector>

using namespace iox2;

template<size_t N>
struct Payload {
    uint8_t data[N];
    uint64_t timestamp_ns;
};

constexpr size_t SIZES[] = {10, 100, 1024, 2048, 4096, 8192};
constexpr uint64_t NUM_MESSAGES = 1000;

template<size_t N>
uint64_t measure(PublisherSubscriber<Payload<N>, SubscriberOnly>& sub) {
    uint64_t total_ns = 0;
    uint64_t count = 0;

    while (count < NUM_MESSAGES) {
        auto recv = sub.receive().expect("recv");
        while (recv.has_value()) {
            auto msg = recv->payload();
            uint64_t now = std::chrono::steady_clock::now().time_since_epoch().count();
            total_ns += now - msg.timestamp_ns;
            count++;
            recv = sub.receive().expect("recv");
        }
    }
    return total_ns / NUM_MESSAGES;
}

int main() {
    set_log_level_from_env_or(LogLevel::Info);

    auto node = NodeBuilder().create<ServiceType::Ipc>().expect("node");
    auto builder =
        node.service_builder(ServiceName::create("Latency/Test").expect("service"))
            .publish_subscribe<Payload<8192>>() // max size
            .open_or_create()
            .expect("service");

    auto sub10   = builder.subscriber_builder<Payload<10>>().create().expect("sub10");
    auto sub100  = builder.subscriber_builder<Payload<100>>().create().expect("sub100");
    auto sub1k   = builder.subscriber_builder<Payload<1024>>().create().expect("sub1k");
    auto sub2k   = builder.subscriber_builder<Payload<2048>>().create().expect("sub2k");
    auto sub4k   = builder.subscriber_builder<Payload<4096>>().create().expect("sub4k");
    auto sub8k   = builder.subscriber_builder<Payload<8192>>().create().expect("sub8k");

    std::cout << "Waiting for messages..." << std::endl;

    auto avg10  = measure(sub10);
    auto avg100 = measure(sub100);
    auto avg1k  = measure(sub1k);
    auto avg2k  = measure(sub2k);
    auto avg4k  = measure(sub4k);
    auto avg8k  = measure(sub8k);

    std::cout << "\n=== Average Latency (ns) ===" << std::endl;
    std::cout << "10 B   : " << avg10  << std::endl;
    std::cout << "100 B  : " << avg100 << std::endl;
    std::cout << "1 KB   : " << avg1k  << std::endl;
    std::cout << "2 KB   : " << avg2k  << std::endl;
    std::cout << "4 KB   : " << avg4k  << std::endl;
    std::cout << "8 KB   : " << avg8k  << std::endl;

    std::cout << "Done." << std::endl;
}
