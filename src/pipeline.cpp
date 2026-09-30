#include "market_data.hpp"

#include <chrono>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

uint64_t now_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void producer(int write_fd) {

    constexpr uint64_t NUM_MESSAGES {1'000'000};

    for (uint64_t i = 0; i < NUM_MESSAGES; ++i) {

        MarketData data{
            .sequence = i,
            .timestamp_ns = now_ns(),
            .price = 100.0 + (i % 100) * 0.01,
            .quantity = 100
        };

        ssize_t result = write(write_fd, &data, sizeof(data));

        if (result != sizeof(data)) {
            std::cerr << "write failed\n";
            std::exit(1);
        }
    }

    close(write_fd);
}

void consumer(int read_fd) {
    uint64_t total_latency {0};
    uint64_t count {0};

    MarketData data {};

    while (true) {
        ssize_t result = read(read_fd, &data, sizeof(data));

        if (result <= 0) {
            break;
        }

        if (result != sizeof(data)) {
            std::cerr << "partial read\n";
            std::exit(1);
        }

        uint64_t received = now_ns();
        uint64_t latency = received - data.timestamp_ns;
        total_latency += latency;

        ++count;
    }

    close(read_fd);

    if (count > 0) {
        double avg_ns = static_cast<double>(total_latency) / count;

        std::cout << "Messages: " << count << '\n';
        std::cout << "Average latency: " << avg_ns / 1000.0 << " us\n";
    }
}

int main() {

    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        std::cerr << "pipe() failed\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "fork() failed\n";
        return 1;
    }

    if (pid == 0) {  // Child = Consumer
        close(pipe_fd[1]);
        consumer(pipe_fd[0]);
    } else {         // Parent = Producer
        close(pipe_fd[0]);
        producer(pipe_fd[1]);
        waitpid(pid, nullptr, 0);
    }

    return 0;
}

