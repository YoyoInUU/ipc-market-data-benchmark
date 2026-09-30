#include "benchmark.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

// Timing
void Benchmark::start() {
    start_time_ = Clock::now();
}

uint64_t Benchmark::elapsed_ns() const {
    const auto elapsed {Clock::now() - start_time_};

    return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
}

// Recording
void Benchmark::record_latency(uint64_t latency_ns) {
    latencies_.push_back(latency_ns);
}

// Statistics
double Benchmark::average_latency_ns() const {
    if (latencies_.empty()) {
        return 0.0;
    }

    uint64_t total {0};

    for (const auto latency : latencies_) {
        total += latency;
    }

    return static_cast<double>(total) / static_cast<double>(latencies_.size());
}

uint64_t Benchmark::p50_latency_ns() const {
    return percentile_ns(0.50);
}

uint64_t Benchmark::p99_latency_ns() const {
    return percentile_ns(0.99);
}

double Benchmark::throughput(uint64_t message_count) const {
    const auto elapsed {elapsed_ns()};

    if (elapsed == 0 || message_count == 0) {
        return 0.0;
    }

    const double elapsed_seconds {static_cast<double>(elapsed) / 1'000'000'000.0};

    return static_cast<double>(message_count) / elapsed_seconds;
}

// Output
void Benchmark::print_results(uint64_t message_count) const {
    std::cout << "Elapsed: " << elapsed_ns() << " ns\n";
    std::cout << "Average latency: " << average_latency_ns() << " ns\n";
    std::cout << "P50 latency: " << p50_latency_ns() << " ns\n";
    std::cout << "P99 latency: " << p99_latency_ns() << " ns\n";
    std::cout << "Throughput: " << throughput(message_count) << " msg/s\n";
}

// Private
uint64_t Benchmark::percentile_ns(double percentile) const {
    if (latencies_.empty()) {
        return 0;
    }

    if (percentile < 0.0 || percentile > 1.0) {
        throw std::invalid_argument("percentile must be between 0 and 1");
    }

    auto samples {latencies_};

    std::sort(samples.begin(), samples.end());

    const auto index {
        static_cast<std::size_t>(
            percentile * static_cast<double>(samples.size() - 1)
        )
    };

    return samples[index];
}