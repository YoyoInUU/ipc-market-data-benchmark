cmake_minimum_required(VERSION 3.16)

project(low_latency_ipc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ipc_benchmark
    src/pipeline.cpp
)

target_include_directories(ipc_benchmark
    PRIVATE
    include
)
