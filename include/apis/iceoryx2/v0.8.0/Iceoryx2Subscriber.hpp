#include <functional>
#include <vector>

#include "api/ReelaySubscriber.hpp"
#include "iox2/iceoryx2.hpp"

constexpr iox2::bb::Duration CYCLE_TIME = iox2::bb::Duration::from_millis(1);

using incoming_message_type = std::vector<uint8_t>;
using namespace iox2;

class Iceoryx2Subscriber : public ReelaySubscriber<incoming_message_type> {
 public:
  Iceoryx2Subscriber(const std::string& key)
      : key(key),
        node(iox2::NodeBuilder().create<iox2::ServiceType::Ipc>().value()),
        service(node
                  .service_builder(
                    ServiceName::create("Service With Dynamic Data").value())
                  .publish_subscribe<bb::Slice<uint8_t>>()
                  .open_or_create()
                  .value()),
        subscriber(service.subscriber_builder().create().value())
  {
  }

  void start(
    std::function<void(const incoming_message_type&)> callback) override
  {
    while(node.wait(CYCLE_TIME).has_value()) {
      auto sample = subscriber.receive().value();
      while(sample.has_value()) {
        auto payload = sample->payload();
        std::vector<uint8_t> payload_v(payload.begin(), payload.end());
        callback(payload_v);
        sample = subscriber.receive().value();
      }
    }
  }

 private:
  std::string key;
  iox2::Node<iox2::ServiceType::Ipc> node;
  iox2::PortFactoryPublishSubscribe<
    iox2::ServiceType::Ipc,
    iox2::bb::Slice<uint8_t>,
    void>
    service;
  iox2::Subscriber<iox2::ServiceType::Ipc, iox2::bb::Slice<uint8_t>, void>
    subscriber;
};
