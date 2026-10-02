#pragma once

#include "config.hpp"
#include "market_data.hpp"

#include <atomic>
#include <cstddef>

namespace ipc {

class RingBuffer {
public:
    RingBuffer();

    bool try_push(const MarketData& data);
    bool try_pop(MarketData& data);

    bool empty() const;
    bool full() const;

    std::size_t size() const;

private:
    std::atomic<std::size_t> head_;
    std::atomic<std::size_t> tail_;

    MarketData buffer_[RING_SIZE];
};

} // namespace ipc