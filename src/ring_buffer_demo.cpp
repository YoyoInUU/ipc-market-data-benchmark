#include "benchmark.hpp"
#include "shared_memory.hpp"

#include <cstdint>
#include <iostream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

int main() {
    ipc::SharedMemory shared_memory;

    if (!shared_memory.create()) {
        std::cerr << "Failed to create shared memory\n";
        return 1;
    }

    auto* shared {shared_memory.data()};
    auto& ring {shared->ring};

    const pid_t pid {fork()};

    if (pid == -1) {
        std::cerr << "fork() failed\n";
        return 1;
    }

    if (pid == 0) {
        // Consumer
        Benchmark benchmark;
        benchmark.start();

        for (uint64_t i {0}; i < NUM_MESSAGES; ++i) {
            MarketData data {};

            while (!ring.try_pop(data)) {
                std::this_thread::yield();
            }

            const uint64_t received_at {Benchmark::now_ns()};
            const uint64_t latency {received_at - data.timestamp_ns};

            benchmark.record_latency(latency);
        }

        benchmark.print_results(NUM_MESSAGES);

        return 0;
    }

    // Producer
    for (uint64_t i {0}; i < NUM_MESSAGES; ++i) {
        MarketData data {};

        data.sequence = i;
        data.timestamp_ns = Benchmark::now_ns();

        while (!ring.try_push(data)) {
            std::this_thread::yield();
        }
    }

    waitpid(pid, nullptr, 0);

    shared_memory.unlink();

    return 0;
}