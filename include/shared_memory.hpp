#pragma once

#include "market_data.hpp"

#include <atomic>
#include <cstddef>

namespace ipc {

struct SharedMemoryData {
    std::atomic<bool> ready{false};
    MarketData data{};
};

class SharedMemory {
public:
    SharedMemory() = default;
    ~SharedMemory();

    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

    bool create();
    bool open();

    bool write(const MarketData& data);
    bool read(MarketData& data);

    void close();
    static void unlink();

private:
    int fd_{-1};
    SharedMemoryData* memory_{nullptr};
};

} // namespace ipc