cmake_minimum_required(VERSION 3.10)
project(CryptoBacktester VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/strategies
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backtester
)

# Source files
set(SOURCES
    src/main.cpp
    src/data/data_loader.cpp
    src/models/asset.cpp
    src/strategies/mean_reversion.cpp
    src/backtester/backtester.cpp
)

# Create executable
add_executable(CryptoBacktester ${SOURCES})

# Configure test subdirectory
#add_subdirectory(test)