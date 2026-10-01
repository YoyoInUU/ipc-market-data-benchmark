#include "benchmark.hpp"
#include "config.hpp"
#include "market_data.hpp"
#include "shared_memory.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    ipc::SharedMemory shared_memory;

    if (!shared_memory.open()) {
        std::cerr << "Failed to open shared memory\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork() failed\n";
        return 1;
    }

    if (pid == 0) {
        // Producer
        for (uint64_t i = 0; i < NUM_MESSAGES; ++i) {
            MarketData data{};
            data.timestamp_ns = Benchmark::Clock::now()
                                    .time_since_epoch()
                                    .count();

            shared_memory.write(data);
        }

        return 0;
    }

    // Consumer
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

    waitpid(pid, nullptr, 0);
    shared_memory.close();

    return 0;
}