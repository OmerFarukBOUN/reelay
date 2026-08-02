#pragma once

template<typename MsgT>
class ReelayPublisher {
public:
    virtual ~ReelayPublisher() = default;
    virtual void publish(const MsgT& msg) = 0;
};
