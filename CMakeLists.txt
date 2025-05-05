cmake_minimum_required(VERSION 3.10)
project(CryptoTradingStrategyBacktester)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories - more simple approach
include_directories(include)

# Find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(backtester ${SOURCES})