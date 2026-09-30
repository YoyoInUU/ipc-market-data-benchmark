# IPC Market Data Benchmark

A small C++ project for learning and benchmarking Inter-Process Communication (IPC) using a simulated market data pipeline.

The project starts with a simple POSIX Pipe implementation and will gradually evolve to compare different IPC mechanisms and low-latency communication techniques.

## 🎯 Project Goals

This project is designed to explore:
- Inter-Process Communication (IPC)
- C++ systems programming
- Process-based producer/consumer pipelines
- IPC latency
- Throughput
- Low-latency programming concepts
- Performance benchmarking

The project is educational and is not intended to be a production trading system.

## 🏗️ Architecture

**v1 - POSIX Pipe**

The first version uses two processes:
```
┌──────────────┐
│   Producer   │
│    Process   │
└──────┬───────┘
       │
       │ POSIX Pipe
       │
       ▼
┌──────────────┐
│   Consumer   │
│    Process   │
└──────────────┘
```

- The Producer generates simulated market data and sends it through a POSIX pipe.
- The Consumer receives the data and measures communication latency.

### Compilation

**File Structure**

```
ipc-market-data-benchmark/
├── CMakeLists.txt
├── include/
│   └── market_data.hpp
└── src/
    └── pipeline.cpp
```

**Requirements**

- Linux
- C++20 compatible compiler
- CMake 3.16+

**Build**

1. Create a build directory:
```
mkdir build
cd build
```
2. Configure the project:
```
cmake ..
```
3. Build:
```
make
```
4. Run:
```
./ipc_benchmark
```

The benchmark result will vary depending on the machine and runtime environment.

Factors that can affect latency include:
- CPU architecture
- CPU frequency
- Operating system
- System load
- Process scheduling
- Compiler and optimization settings
- Cache behavior

Therefore, benchmark results should not be treated as universal performance numbers.

## 📄 License

This project is for educational purposes. Feel free to use and modify as needed.

---

**Author**: Yoyo Chang  
**Last Updated**: September 2026  
**C++ Standard**: C++20