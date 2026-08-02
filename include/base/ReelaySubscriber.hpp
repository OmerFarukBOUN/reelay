#pragma once

#include <functional>

template<typename MsgT>
class ReelaySubscriber {
public:
    virtual ~ReelaySubscriber() = default;
    virtual void start(std::function<void(const MsgT&)> callback) = 0;
};
