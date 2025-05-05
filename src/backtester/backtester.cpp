#include "backtester.h"
#include <cmath>
#include <algorithm>
#include <numeric>

Backtester::Backtester(double capital, double position_size) 
    : initial_capital(capital), current_capital(capital), position_size_pct(position_size) {}

BacktestResult Backtester::runBacktest(const Asset& asset, Strategy& strategy) {
    std::vector<Signal> signals = strategy.generateSignals(asset);
    auto prices = asset.getClosePrices();
    
    // Reset state
    current_capital = initial_capital;
    equity_curve.clear();
    equity_curve.push_back(current_capital);
    trades.clear();
    
    bool in_position = false;
    bool is_long_position = false;
    double position_size = 0.0;
    double entry_price = 0.0;
    size_t entry_index = 0;
    
    for (size_t i = 1; i < signals.size(); ++i) {
        Signal current_signal = signals[i];
        
        // Handle position exit
        if (in_position) {
            if ((is_long_position && current_signal == SELL) || 
                (!is_long_position && current_signal == BUY)) {
                
                double exit_price = prices[i];
                double profit = 0.0;
                
                if (is_long_position) {
                    profit = position_size * (exit_price / entry_price - 1.0);
                } else {
                    profit = position_size * (entry_price / exit_price - 1.0);
                }
                
                current_capital += profit;
                
                Trade trade;
                trade.entry_index = entry_index;
                trade.exit_index = i;
                trade.entry_price = entry_price;
                trade.exit_price = exit_price;
                trade.profit_pct = 100.0 * profit / position_size;
                trade.is_long = is_long_position;
                
                recordTrade(trade);
                
                in_position = false;
            }
        }
        
        // Handle position entry
        if (!in_position) {
            if (current_signal == BUY) {
                entry_price = prices[i];
                position_size = current_capital * position_size_pct;
                entry_index = i;
                in_position = true;
                is_long_position = true;
            } else if (current_signal == SELL) {
                entry_price = prices[i];
                position_size = current_capital * position_size_pct;
                entry_index = i;
                in_position = true;
                is_long_position = false;
            }
        }
        
        equity_curve.push_back(current_capital);
    }
    
    // Close any open position at the end of the backtest
    if (in_position) {
        double exit_price = prices.back();
        double profit = 0.0;
        
        if (is_long_position) {
            profit = position_size * (exit_price / entry_price - 1.0);
        } else {
            profit = position_size * (entry_price / exit_price - 1.0);
        }
        
        current_capital += profit;
        
        Trade trade;
        trade.entry_index = entry_index;
        trade.exit_index = prices.size() - 1;
        trade.entry_price = entry_price;
        trade.exit_price = exit_price;
        trade.profit_pct = 100.0 * profit / position_size;
        trade.is_long = is_long_position;
        
        recordTrade(trade);
        
        equity_curve.back() = current_capital;
    }
    
    // Calculate performance metrics
    BacktestResult result;
    result.strategy_name = strategy.getName();
    result.symbol = asset.getSymbol();
    result.initial_capital = initial_capital;
    result.final_capital = current_capital;
    result.total_return_pct = 100.0 * (current_capital / initial_capital - 1.0);
    
    // Calculate daily returns from equity curve
    std::vector<double> daily_returns;
    for (size_t i = 1; i < equity_curve.size(); i++) {
        daily_returns.push_back(equity_curve[i] / equity_curve[i-1] - 1.0);
    }
    
    result.sharpe_ratio = calculateSharpeRatio(daily_returns);
    result.max_drawdown_pct = calculateMaxDrawdown(equity_curve);
    result.total_trades = trades.size();
    
    // Count winning trades
    result.winning_trades = std::count_if(trades.begin(), trades.end(), 
        [](const Trade& t) { return t.profit_pct > 0.0; });
    
    result.win_rate = static_cast<double>(result.winning_trades) / result.total_trades;
    
    // Calculate average profit per trade
    double total_profit = std::accumulate(trades.begin(), trades.end(), 0.0,
        [](double sum, const Trade& t) { return sum + t.profit_pct; });
    
    result.avg_profit_per_trade = total_profit / result.total_trades;
    result.equity_curve = equity_curve;
    result.trades = trades;
    
    return result;
}

double Backtester::calculateSharpeRatio(const std::vector<double>& returns, double risk_free_rate) const {
    if (returns.empty()) {
        return 0.0;
    }
    
    double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
    double mean = sum / returns.size();
    
    double sq_sum = std::inner_product(returns.begin(), returns.end(), returns.begin(), 0.0,
                                     std::plus<>(),
                                     [mean](double a, double b) {
                                         return (a - mean) * (b - mean);
                                     });
    
    double std_dev = std::sqrt(sq_sum / returns.size());
    
    // Annualize (assuming daily returns)
    double annualized_return = (mean - risk_free_rate) * 252.0;
    double annualized_volatility = std_dev * std::sqrt(252.0);
    
    return annualized_volatility == 0.0 ? 0.0 : annualized_return / annualized_volatility;
}

double Backtester::calculateMaxDrawdown(const std::vector<double>& equity) const {
    double max_drawdown = 0.0;
    double peak = equity[0];
    
    for (double value : equity) {
        if (value > peak) {
            peak = value;
        } else {
            double drawdown = (peak - value) / peak;
            max_drawdown = std::max(max_drawdown, drawdown);
        }
    }
    
    return 100.0 * max_drawdown;
}

void Backtester::recordTrade(const Trade& trade) {
    trades.push_back(trade);
}