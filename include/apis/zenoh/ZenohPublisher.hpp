#include "api/ReelayPublisher.hpp"
#include "zenoh.hxx"
#include "reelay/monitors.hpp"

using monitor_output_type = reelay::json;

class ZenohPublisher : public ReelayPublisher<monitor_output_type> {
public:
    ZenohPublisher(zenoh::Session& session, const std::string& key)
        : pub(session.declare_publisher(key)) {}

    void publish(const monitor_output_type& msg) override {
        pub.put(msg.dump());
    }

private:
    zenoh::Publisher pub;
};
