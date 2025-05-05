#pragma once
#include "models/asset.h"
#include "strategies/strategy.h"
#include <vector>
#include <string>

struct Trade {
    size_t entry_index;
    size_t exit_index;
    double entry_price;
    double exit_price;
    double profit_pct;
    bool is_long;
};

struct BacktestResult {
    std::string strategy_name;
    std::string symbol;
    double initial_capital;
    double final_capital;
    double total_return_pct;
    double sharpe_ratio;
    double max_drawdown_pct;
    int total_trades;
    int winning_trades;
    double win_rate;
    double avg_profit_per_trade;
    std::vector<double> equity_curve;
    std::vector<Trade> trades;
};

class Backtester {
private:
    double initial_capital;
    double current_capital;
    double position_size_pct;
    std::vector<double> equity_curve;
    std::vector<Trade> trades;
    
public:
    Backtester(double capital = 10000.0, double position_size = 0.1);
    
    BacktestResult runBacktest(const Asset& asset, Strategy& strategy);
    
private:
    double calculateSharpeRatio(const std::vector<double>& returns, double risk_free_rate = 0.0) const;
    double calculateMaxDrawdown(const std::vector<double>& equity) const;
    void recordTrade(const Trade& trade);
};