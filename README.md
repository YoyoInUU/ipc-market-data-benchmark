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

**v2 - Shared Memory**

- The Producer and Consumer exchange market data through shared memory using `shm_open()` and `mmap()`.
- An atomic `ready` flag synchronizes access using busy-waiting.
- The Consumer measures communication latency for comparison with the POSIX pipe version.

### Compilation

**File Structure**

```
ipc-market-data-benchmark/
├── CMakeLists.txt
├── include/
│   ├── benchmark.hpp
│   ├── config.hpp
│   ├── market_data.hpp
│   └── shared_memory.hpp
└── src/
    ├── benchmark.cpp
    ├── pipeline.cpp
    ├── shared_memory.cpp
    └── shared_memory_demo.cpp
```

**Requirements**

- Linux, either natively or through WSL 2 on Windows
- C++20 compatible compiler
- CMake 3.16+

This project uses POSIX APIs such as `fork()`, `pipe()`, `shm_open()`, and `mmap()`. Native Windows builds with MSVC or MinGW are not supported. Git Bash alone does not provide the Linux environment required to build and run this project.

**Build on Linux**

1. Create a build directory:
```bash
mkdir build
cd build
```
2. Configure the project:
```bash
cmake ..
```
3. Build:
```bash
make
```
4. Run either benchmark:
```bash
# POSIX pipeline
./pipeline

# Shared memory
./shared_memory_demo
```

**Build on Windows using WSL 2**

1. If WSL is not installed, open PowerShell as Administrator and run:
```powershell
wsl --install -d Ubuntu-24.04 --web-download
```

Restart Windows if prompted, then open Ubuntu and complete its initial setup. Run all commands below in the **Ubuntu terminal**, not PowerShell, Command Prompt, or Git Bash.

2. Install the build tools:
```bash
sudo apt update
sudo apt install build-essential cmake
```

3. Navigate to the project. Use a separate build directory to avoid reusing a CMake cache generated on Windows:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

**Troubleshooting: CMake reuses a Windows cache**

If `cmake ..` reports a cache path or generator mismatch, the build directory may contain a cache created by Windows CMake. Even a directory named `build-wsl` can contain a Windows cache if it was configured outside WSL.

From inside the affected build directory, run these commands in the **Ubuntu terminal**:

```bash
cmake --fresh -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
```

`--fresh` requires CMake 3.24+ and resets `CMakeCache.txt` and `CMakeFiles/`. With older CMake versions, use a new, empty build directory instead. After the reset, normal `cmake ..` runs work again; there is no need to reset the cache on every build.

The `build/` configuration, build, and both benchmarks were verified in WSL with GNU C++ 13.3.0 and CMake 3.28.3 using the Release configuration.

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