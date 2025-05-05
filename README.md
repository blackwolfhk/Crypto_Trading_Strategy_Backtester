# Crypto Trading Strategy Backtester

A C++ application for backtesting quantitative trading strategies on cryptocurrency data. This project uses a modular, organized structure following best practices for C++ project organization.

Features

- Data handling for historical cryptocurrency prices
- Implementation of mean reversion strategy using Bollinger Bands
- Comprehensive backtesting engine with performance metrics
- Trade tracking and performance visualization
- Modular design for easy extension with new strategies

## Project Overview

This backtester allows you to test trading strategies against historical Bitcoin price data. The repository includes data from Bitstamp's BTC/USD daily prices from 2016 to 2025, enabling comprehensive strategy evaluation across different market conditions.

Requirements
C++17 compatible compiler
CMake 3.10 or higher
Setting Up CMake
bash

### Install CMake (on macOS with Homebrew)

brew install cmake

### Add CMake to your path by adding this to your ~/.zshrc file

export PATH="/Applications/CMake.app/Contents/bin:$PATH"

### Reload your shell config

source ~/.zshrc

### Verify installation

cmake --version

# Building and Running the Project

## Clone the repository

git clone https://github.com/blackwolfhk/Crypto_Trading_Strategy_Backtester

cd Crypto_Trading_Strategy_Backtester

### Create build directory

mkdir -p build
cd build

### Configure and build

cmake ..
cmake --build .

### Make sure data directory exists

mkdir -p data

### Copy the Bitcoin data file (if needed)

cp ../data/btc_historical.csv ./data/

### Run the backtester

./backtester

### Data Source

Historical Bitcoin price data is sourced from Bitstamp via CryptoDataDownload.
The data includes daily OHLCV (Open, High, Low, Close, Volume) information.

### Alternative way to download the latest Bitcoin historical data

curl -o data/btc_historical.csv "https://www.cryptodatadownload.com/cdd/Bitstamp_BTCUSD_d.csv"
