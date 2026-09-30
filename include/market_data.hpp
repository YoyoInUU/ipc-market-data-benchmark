#pragma once

#include <cstdint>

struct MarketData {
    uint64_t sequence;
    uint64_t timestamp_ns;
    double price;
    uint32_t quantity;
};