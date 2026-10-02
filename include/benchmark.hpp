#pragma once

#include <chrono>
#include <cstdint>
#include <vector>

class Benchmark {
public:
    using Clock = std::chrono::steady_clock;

    // Timing
    static uint64_t now_ns();
    void start();
    uint64_t elapsed_ns() const;

    // Recording
    void record_latency(uint64_t latency_ns);

    // Statistics
    double average_latency_ns() const;
    uint64_t p50_latency_ns() const;
    uint64_t p99_latency_ns() const;
    double throughput(uint64_t message_count) const;

    // Output
    void print_results(uint64_t message_count) const;

private:
    uint64_t percentile_ns(double percentile) const;

    Clock::time_point start_time_{};
    std::vector<uint64_t> latencies_;
};