#include "backtester/backtester.h"
#include "strategies/sma_strategy.h"
#include "strategies/rsi_strategy.h"
#include "strategies/bollinger_bands_strategy.h"
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
    std::cout << "===== Cryptocurrency Trading Strategy Backtester =====\n\n";
    
    // Get data path
    std::string dataPath = "data/btc_historical.csv";
    if (argc > 1) {
        dataPath = argv[1];
    }
    
    // Check if data file exists
    if (!std::filesystem::exists(dataPath)) {
        std::cerr << "Error: Data file not found at " << dataPath << std::endl;
        std::cerr << "Please download the Bitcoin historical data first using the download_data.py script." << std::endl;
        return 1;
    }
    
    // Initialize backtester
    crypto::backtester::Backtester backtester(dataPath);
    
    // Create strategies
    auto smaStrategy1 = std::make_shared<crypto::strategies::SMAStrategy>(20, 50);
    auto smaStrategy2 = std::make_shared<crypto::strategies::SMAStrategy>(50, 200);
    auto rsiStrategy = std::make_shared<crypto::strategies::RSIStrategy>(14, 30, 70);
    auto bbStrategy = std::make_shared<crypto::strategies::BollingerBandsStrategy>(20, 2.0);
    
    // Add strategies to backtester
    backtester.addStrategy(smaStrategy1);
    backtester.addStrategy(smaStrategy2);
    backtester.addStrategy(rsiStrategy);
    backtester.addStrategy(bbStrategy);
    
    // Run backtest
    backtester.run(10000.0, 0.95);  // Initial capital of $10,000, invest 95% of capital per trade
    
    // Compare strategies
    backtester.compareStrategies();
    
    // Export results for visualization
    backtester.exportResults(".");
    
    std::cout << "\nBacktest complete. Results have been exported.\n";
    std::cout << "To visualize the results, run the visualize_results.py script.\n";
    
    return 0;
}