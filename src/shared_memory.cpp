#include "shared_memory.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace ipc {

SharedMemory::~SharedMemory() {
    close();
}

bool SharedMemory::create() {
    fd_ = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);

    if (fd_ == -1) {
        return false;
    }

    if (ftruncate(fd_, sizeof(SharedMemoryData)) == -1) {
        close();
        return false;
    }

    void* ptr = mmap(nullptr, sizeof(SharedMemoryData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);

    if (ptr == MAP_FAILED) {
        close();
        return false;
    }

    memory_ = static_cast<SharedMemoryData*>(ptr);

    return true;
}

bool SharedMemory::open() {
    fd_ = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);

    if (fd_ == -1) {
        return false;
    }

    if (ftruncate(fd_, sizeof(SharedMemoryData)) == -1) {
        close();
        return false;
    }

    void* ptr = mmap(nullptr, sizeof(SharedMemoryData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);

    if (ptr == MAP_FAILED) {
        close();
        return false;
    }

    memory_ = static_cast<SharedMemoryData*>(ptr);

    return true;
}

bool SharedMemory::write(const MarketData& data) {
    if (memory_ == nullptr) {
        return false;
    }

    while (memory_->ready.load(std::memory_order_acquire)) {
    }

    memory_->data = data;
    memory_->ready.store(true, std::memory_order_release);

    return true;
}

bool SharedMemory::read(MarketData& data) {
    if (memory_ == nullptr) {
        return false;
    }

    while (!memory_->ready.load(std::memory_order_acquire)) {
    }

    data = memory_->data;
    memory_->ready.store(false, std::memory_order_release);

    return true;
}

void SharedMemory::close() {
    if (memory_ != nullptr) {
        munmap(memory_, sizeof(SharedMemoryData));
        memory_ = nullptr;
    }

    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

void SharedMemory::unlink() {
    shm_unlink(SHARED_MEMORY_NAME);
}

} // namespace ipc