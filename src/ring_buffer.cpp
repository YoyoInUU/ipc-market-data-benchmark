#include "ring_buffer.hpp"

namespace ipc {

RingBuffer::RingBuffer() : head_(0), tail_(0) {
}

bool RingBuffer::try_push(const MarketData& data) {
    const std::size_t head = head_.load(std::memory_order_relaxed);
    const std::size_t next = (head + 1) % RING_SIZE;

    // Buffer is full
    if (next == tail_.load(std::memory_order_acquire)) {
        return false;
    }

    buffer_[head] = data;

    // Publish the new element
    head_.store(next, std::memory_order_release);

    return true;
}

bool RingBuffer::try_pop(MarketData& data) {
    const std::size_t tail = tail_.load(std::memory_order_relaxed);

    // Buffer is empty
    if (tail == head_.load(std::memory_order_acquire)) {
        return false;
    }

    data = buffer_[tail];
    const std::size_t next = (tail + 1) % RING_SIZE;

    // Release the slot
    tail_.store(next, std::memory_order_release);

    return true;
}

bool RingBuffer::empty() const {
    return tail_.load(std::memory_order_relaxed) == head_.load(std::memory_order_acquire);
}

bool RingBuffer::full() const {
    const std::size_t head = head_.load(std::memory_order_relaxed);
    const std::size_t next = (head + 1) % RING_SIZE;

    return next == tail_.load(std::memory_order_acquire);
}

std::size_t RingBuffer::size() const {
    const std::size_t head = head_.load(std::memory_order_acquire);
    const std::size_t tail = tail_.load(std::memory_order_acquire);

    if (head >= tail) {
        return head - tail;
    }

    return RING_SIZE - tail + head;
}

} // namespace ipc