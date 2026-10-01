cmake_minimum_required(VERSION 3.16)

project(low_latency_ipc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Benchmark library
add_library(benchmark
    src/benchmark.cpp
)

target_include_directories(benchmark
    PUBLIC
    include
)

# Pipeline IPC
add_executable(pipeline
    src/pipeline.cpp
)

target_link_libraries(pipeline PRIVATE
    benchmark
)

# Shared Memory IPC
add_executable(shared_memory_demo
    src/shared_memory_demo.cpp
    src/shared_memory.cpp
)

target_link_libraries(shared_memory_demo PRIVATE
    benchmark
)