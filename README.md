# Crypto Trading Strategy Backtester

A C++ application for backtesting quantitative trading strategies on cryptocurrency data. This project uses a modular, organized structure following best practices for C++ project organization.

## Features

- Data handling for historical cryptocurrency prices
- Implementation of mean reversion strategy using Bollinger Bands
- Comprehensive backtesting engine with performance metrics
- Trade tracking and performance visualization
- Modular design for easy extension with new strategies

## Requirements

### Install CMake

brew install cmake

### Add CMake to your path by adding this to your ~/.zshrc file

export PATH="/Applications/CMake.app/Contents/bin:$PATH"

### reload your shell config

source ~/.zshrc

cmake --version

- C++17 compatible compiler
- CMake 3.10 or higher

## Building the Project

```
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

# From the build directory

./CryptoBacktester

### download btc data

curl -o data/btc_historical.csv "https://www.cryptodatadownload.com/cdd/Bitstamp_BTCUSD_d.csv"
