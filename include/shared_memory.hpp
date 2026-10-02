#pragma once

#include "market_data.hpp"
#include "ring_buffer.hpp"

#include <atomic>

namespace ipc {

struct SharedMemoryData {
    // v2
    std::atomic<bool> ready {false};
    MarketData data {};

    // v3
    RingBuffer ring;
};

class SharedMemory {
public:
    SharedMemory() = default;
    ~SharedMemory();

    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

    bool create();
    bool open();

    // v2
    bool write(const MarketData& data);
    bool read(MarketData& data);

    // v3
    SharedMemoryData* data();

    void close();
    static void unlink();

private:
    int fd_ {-1};
    SharedMemoryData* memory_ {nullptr};
};

} // namespace ipc