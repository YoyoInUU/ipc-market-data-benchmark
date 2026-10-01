#include "benchmark.hpp"
#include "config.hpp"
#include "market_data.hpp"
#include "shared_memory.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>

int main() {
    ipc::SharedMemory shared_memory;

    if (!shared_memory.open()) {
        std::cerr << "Failed to open shared memory\n";
        return 1;
    }

    std::cout << "Shared memory benchmark started\n";

    Benchmark benchmark;
    benchmark.start();

    for (uint64_t i = 0; i < NUM_MESSAGES; ++i) {
        MarketData data{};

        if (!shared_memory.read(data)) {
            std::cerr << "Failed to read market data\n";
            shared_memory.close();
            return 1;
        }

        const auto now = Benchmark::Clock::now();

        const uint64_t latency_ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                now.time_since_epoch()
            ).count() - data.timestamp_ns;

        benchmark.record_latency(latency_ns);
    }

    benchmark.print_results(NUM_MESSAGES);
    shared_memory.close();

    return 0;
}