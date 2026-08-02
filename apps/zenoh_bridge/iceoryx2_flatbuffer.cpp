#include "base/LTLMonitoringPipeline.hpp"
#include <apis/iceoryx2/v0.8.0/Iceoryx2Publisher.hpp>
#include <apis/iceoryx2/v0.8.0/Iceoryx2Subscriber.hpp>
#include <apis/flatbuffer/FlatbufferParser.hpp>

auto main () -> int {
	auto publisher = Iceoryx2Publisher("/esmini/out");
	auto subscriber = Iceoryx2Subscriber("/reelay/out");
	init_flatbuffers("/pub_sub_reelay/apps/zenoh_bridge/flatbuffers");
	auto parser = FlatbufferParser();
	auto pipeline = reelay::LTLMonitoringPipeline(parser, publisher, subscriber);
}