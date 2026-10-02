#pragma once

#include <cstdint>

constexpr uint64_t NUM_MESSAGES {1'000'000};
constexpr const char* SHM_NAME {"/ipc_market_data"};
constexpr std::size_t RING_SIZE {1024};