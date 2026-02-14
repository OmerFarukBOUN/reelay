#include "reelay/monitors.hpp"

#include "api/ReelayPublisher.hpp"
#include "iox2/iceoryx2.hpp"

using monitor_output_type = reelay::json;
using namespace iox2;
constexpr uint64_t INITIAL_SIZE_HINT = 256;

class Iceoryx2Publisher : public ReelayPublisher<monitor_output_type> {
 public:
  Iceoryx2Publisher(const std::string& key)
      : node(iox2::NodeBuilder().create<iox2::ServiceType::Ipc>().value()),
        service(node
                  .service_builder(
                    ServiceName::create("Service With Dynamic Data").value())
                  .publish_subscribe<bb::Slice<uint8_t>>()
                  .open_or_create()
                  .value()),
        pub(service.publisher_builder()
              .initial_max_slice_len(INITIAL_SIZE_HINT)
              .allocation_strategy(AllocationStrategy::PowerOfTwo)
              .create()
              .value())
  {
  }

  void publish(const monitor_output_type& msg) override
  {
    auto json_dump = msg.dump();
    auto sample = pub.loan_slice_uninit(json_dump.length()).value();
    auto initialized_sample = sample.write_payload(json_dump);
    send(std::move(initialized_sample)).value();
  }

 private:
  iox2::Publisher<iox2::ServiceType::Ipc, iox2::bb::Slice<uint8_t>, void> pub;
  iox2::PortFactoryPublishSubscribe<
    iox2::ServiceType::Ipc,
    iox2::bb::Slice<uint8_t>,
    void>
    service;
  iox2::Node<iox2::ServiceType::Ipc> node;
};
