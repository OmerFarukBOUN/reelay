#pragma once

#include "ReelayParser.hpp"
#include "ReelayPublisher.hpp"
#include "ReelaySubscriber.hpp"
#include "reelay/monitors.hpp"


template<typename SubMsg, typename MonitorMsg, typename PubMsg>
class LTLMonitoringPipeline {
public:
    LTLMonitoringPipeline(
        ReelaySubscriber<SubMsg>* subscriber,
        ReelayParser<SubMsg, MonitorMsg>* parser,
        reelay::monitor<MonitorMsg, PubMsg>* monitor,
        ReelayPublisher<PubMsg>* publisher)
        : sub(subscriber), parser(parser), monitor(monitor), pub(publisher) {}

    void run() {
        sub->start([this](const SubMsg& raw){
            MonitorMsg m_in = parser->parse(raw);
            auto m_out = monitor->evaluate(m_in);
            pub->publish(m_out);
        });
    }

private:
    ReelaySubscriber<SubMsg>* sub;
    ReelayParser<SubMsg, MonitorMsg>* parser;
    reelay::monitor<MonitorMsg, PubMsg>* monitor;
    ReelayPublisher<PubMsg>* pub;
};
