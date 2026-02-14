#include "iox2/iceoryx2.hpp"
#include <iostream>
#include <chrono>

using namespace iox2;

template<size_t N>
struct Payload {
    uint8_t data[N];
    uint64_t timestamp_ns;
};

constexpr size_t SIZES[] = {10, 100, 1024, 2048, 4096, 8192};
constexpr uint64_t NUM_MESSAGES = 1000;

template<size_t N>
void send_payloads(Publisher<Payload<N>>& publisher) {
    for (uint64_t i = 0; i < NUM_MESSAGES; i++) {
        auto sample = publisher.loan_uninit().expect("loan");
        auto t = std::chrono::steady_clock::now().time_since_epoch().count();

        Payload<N> payload{};
        payload.timestamp_ns = static_cast<uint64_t>(t);

        auto initialized = sample.write_payload(payload);
        send(std::move(initialized)).expect("send");
    }
}

int main() {
    set_log_level_from_env_or(LogLevel::Info);

    auto node = NodeBuilder().create<ServiceType::Ipc>().expect("node");
    auto builder =
        node.service_builder(ServiceName::create("Latency/Test").expect("service"))
            .publish_subscribe<Payload<8192>>() // max type only matters for memory
            .open_or_create()
            .expect("service");

    // Individual publishers for each size
    auto pub10   = builder.publisher_builder<Payload<10>>().create().expect("pub10");
    auto pub100  = builder.publisher_builder<Payload<100>>().create().expect("pub100");
    auto pub1k   = builder.publisher_builder<Payload<1024>>().create().expect("pub1k");
    auto pub2k   = builder.publisher_builder<Payload<2048>>().create().expect("pub2k");
    auto pub4k   = builder.publisher_builder<Payload<4096>>().create().expect("pub4k");
    auto pub8k   = builder.publisher_builder<Payload<8192>>().create().expect("pub8k");

    std::cout << "Starting transmission..." << std::endl;

    send_payloads(pub10);
    send_payloads(pub100);
    send_payloads(pub1k);
    send_payloads(pub2k);
    send_payloads(pub4k);
    send_payloads(pub8k);

    std::cout << "Publisher finished." << std::endl;
}
