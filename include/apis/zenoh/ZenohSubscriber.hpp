#include "base/ReelaySubscriber.hpp"
#include <vector>
#include "zenoh.hxx"
#include <functional>

using incoming_message_type = std::vector<uint8_t>;

class ZenohSubscriber : public ReelaySubscriber<incoming_message_type> {
public:
    ZenohSubscriber(zenoh::Session& session, const std::string& key)
        : session(session), key(key) {}

    void start(std::function<void(const incoming_message_type&)> callback) override {
        auto sub = session.declare_subscriber(key, 
            [callback](const zenoh::Sample& sample) {
                auto msg = sample.get_payload().as_vector();
                callback(msg);
            }, zenoh::closures::none);
    }

private:
    zenoh::Session& session;
    std::string key;
};
