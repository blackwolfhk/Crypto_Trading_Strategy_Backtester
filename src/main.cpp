#include <iostream>
#include <iomanip>
#include "data/data_loader.h"
#include "models/asset.h"
#include "strategies/mean_reversion.h"
#include "backtester/backtester.h"

void printResults(const BacktestResult& result) {
    std::cout << std::fixed << std::setprecision(2);
    
    std::cout << "\n======== BACKTEST RESULTS ========\n";
    std::cout << "Strategy: " << result.strategy_name << "\n";
    std::cout << "Symbol: " << result.symbol << "\n";
    std::cout << "Initial Capital: $" << result.initial_capital << "\n";
    std::cout << "Final Capital: $" << result.final_capital << "\n";
    std::cout << "Total Return: " << result.total_return_pct << "%\n";
    std::cout << "Sharpe Ratio: " << result.sharpe_ratio << "\n";
    std::cout << "Max Drawdown: " << result.max_drawdown_pct << "%\n";
    std::cout << "Total Trades: " << result.total_trades << "\n";
    std::cout << "Winning Trades: " << result.winning_trades << "\n";
    std::cout << "Win Rate: " << (result.win_rate * 100.0) << "%\n";
    std::cout << "Avg Profit Per Trade: " << result.avg_profit_per_trade << "%\n";
    
    std::cout << "\n======== TRADE SUMMARY ========\n";
    for (size_t i = 0; i < std::min(size_t(10), result.trades.size()); i++) {
        const auto& trade = result.trades[i];
        std::cout << "Trade " << (i+1) << ": " 
                  << (trade.is_long ? "Long" : "Short")
                  << " Entry: $" << trade.entry_price
                  << " Exit: $" << trade.exit_price
                  << " Profit: " << trade.profit_pct << "%\n";
    }
    
    if (result.trades.size() > 10) {
        std::cout << "... and " << (result.trades.size() - 10) << " more trades.\n";
    }
}

int main() {
    std::cout << "Crypto Trading Strategy Backtester\n";
    std::cout << "==================================\n";
    
// Load data
DataLoader loader;
std::string dataPath = "../data/btc_sample.csv";
std::vector<OHLCV> btcData = loader.loadCSV(dataPath);
    
    if (btcData.empty()) {
        std::cerr << "Failed to load data or no data available.\n";
        std::cerr << "Make sure the file exists and contains valid data.\n";
        return 1;
    }
    
    std::cout << "Loaded " << btcData.size() << " price candles.\n";
    
    // Create asset
    Asset btc("BTC", btcData);
    
    // Create strategies
    MeanReversionStrategy meanRevStrategy(5, 1.5);  // 5-day SMA, 1.5 std dev
    
    // Create backtester
    Backtester backtester(10000.0, 0.5);
    
    // Run backtest
    std::cout << "Running backtest...\n";
    BacktestResult result = backtester.runBacktest(btc, meanRevStrategy);
    
    // Print results
    printResults(result);
    
    return 0;
}