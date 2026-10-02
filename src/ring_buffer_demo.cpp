#include "benchmark.hpp"
#include "ring_buffer.hpp"

#include <cstdint>
#include <iostream>
#include <thread>

int main() {
    ipc::RingBuffer ring;

    Benchmark benchmark;
    benchmark.start();

    std::thread producer([&ring]() {
        for (uint64_t i {0}; i < NUM_MESSAGES; ++i) {
            MarketData data {};

            data.sequence = i;
            data.timestamp_ns = Benchmark::now_ns();

            while (!ring.try_push(data)) {
                std::this_thread::yield();
            }
        }
    });

    std::thread consumer([&ring, &benchmark]() {
        for (uint64_t i {0}; i < NUM_MESSAGES; ++i) {
            MarketData data {};

            while (!ring.try_pop(data)) {
                std::this_thread::yield();
            }

            const uint64_t received_at {Benchmark::now_ns()};
            const uint64_t latency {received_at - data.timestamp_ns};

            benchmark.record_latency(latency);
        }
    });

    producer.join();
    consumer.join();

    benchmark.print_results(NUM_MESSAGES);

    return 0;
}