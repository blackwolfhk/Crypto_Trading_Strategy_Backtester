#include "strategies/strategy.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

namespace crypto {
namespace strategies {

Strategy::Strategy(const std::string& name) 
    : m_name(name), m_totalReturn(0.0), m_annualReturn(0.0), 
      m_sharpeRatio(0.0), m_maxDrawdown(0.0), m_winRate(0.0), m_totalTrades(0) {}

void Strategy::backtest(const data::DataLoader& data, double initialCapital, double positionSize) {
    const auto& priceData = data.getData();
    
    if (priceData.empty()) {
        std::cerr << "Error: No data to backtest" << std::endl;
        return;
    }
    
    std::cout << "Backtesting " << m_name << "..." << std::endl;
    
    // Generate signals if not already generated
    if (m_signals.empty()) {
        m_signals = generateSignals(data);
    }
    
    // Reset equity curve and trades
    m_equityCurve.clear();
    m_equityCurve.resize(priceData.size(), initialCapital);
    m_trades.clear();
    
    double cash = initialCapital;
    double btcHoldings = 0.0;
    
    size_t lastBuyIndex = 0;
    int totalBuySignals = 0;
    int totalSellSignals = 0;
    
    for (size_t i = 1; i < priceData.size(); ++i) {
        // By default, carry over yesterday's equity
        m_equityCurve[i] = m_equityCurve[i-1];
        
        // Check for buy signal
        if (m_signals[i] == BUY && btcHoldings == 0.0) {
            double amount = cash * positionSize;
            btcHoldings = amount / priceData[i].close;
            cash -= amount;
            
            lastBuyIndex = i;
            totalBuySignals++;
        }
        // Check for sell signal
        else if (m_signals[i] == SELL && btcHoldings > 0.0) {
            double amount = btcHoldings * priceData[i].close;
            cash += amount;
            
            // Record the trade
            Trade trade;
            trade.entryIndex = lastBuyIndex;
            trade.exitIndex = i;
            trade.entryPrice = priceData[lastBuyIndex].close;
            trade.exitPrice = priceData[i].close;
            trade.profit = amount - (btcHoldings * trade.entryPrice);
            trade.profitPercent = (trade.exitPrice / trade.entryPrice - 1.0) * 100.0;
            
            m_trades.push_back(trade);
            
            btcHoldings = 0.0;
            totalSellSignals++;
        }
        
        // Update equity curve
        m_equityCurve[i] = cash + (btcHoldings * priceData[i].close);
    }
    
    // If we still have BTC at the end, calculate final equity
    if (btcHoldings > 0.0) {
        double finalEquity = cash + (btcHoldings * priceData.back().close);
        m_equityCurve.back() = finalEquity;
    }
    
    // Calculate performance metrics
    m_totalReturn = (m_equityCurve.back() / initialCapital - 1.0) * 100.0;
    
    // Calculate annualized return assuming 365 days per year
    double days = static_cast<double>(priceData.size());
    double years = days / 365.0;
    m_annualReturn = (std::pow(1.0 + m_totalReturn / 100.0, 1.0 / years) - 1.0) * 100.0;
    
    // Calculate max drawdown
    double peak = initialCapital;
    double maxDrawdown = 0.0;
    
    for (const auto& equity : m_equityCurve) {
        if (equity > peak) {
            peak = equity;
        }
        
        double drawdown = (peak - equity) / peak * 100.0;
        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }
    m_maxDrawdown = maxDrawdown;
    
    // Calculate win rate
    m_totalTrades = static_cast<int>(m_trades.size());
    int winningTrades = 0;
    
    for (const auto& trade : m_trades) {
        if (trade.profit > 0.0) {
            winningTrades++;
        }
    }
    
    m_winRate = m_totalTrades > 0 ? static_cast<double>(winningTrades) / m_totalTrades * 100.0 : 0.0;
    
    // Calculate Sharpe ratio (assuming daily data and risk-free rate of 0)
    if (m_equityCurve.size() > 1) {
        std::vector<double> dailyReturns;
        for (size_t i = 1; i < m_equityCurve.size(); ++i) {
            dailyReturns.push_back(m_equityCurve[i] / m_equityCurve[i-1] - 1.0);
        }
        
        double meanReturn = std::accumulate(dailyReturns.begin(), dailyReturns.end(), 0.0) / dailyReturns.size();
        
        double variance = 0.0;
        for (const auto& ret : dailyReturns) {
            variance += (ret - meanReturn) * (ret - meanReturn);
        }
        variance /= dailyReturns.size();
        
        double stdDev = std::sqrt(variance);
        
        // Annualize Sharpe ratio (assuming 252 trading days per year)
        m_sharpeRatio = stdDev > 0 ? (meanReturn / stdDev) * std::sqrt(252.0) : 0.0;
    }
    
    // Print summary
    std::cout << "=== " << m_name << " Performance ===\n";
    std::cout << "Total Return: " << m_totalReturn << "%\n";
    std::cout << "Annual Return: " << m_annualReturn << "%\n";
    std::cout << "Max Drawdown: " << m_maxDrawdown << "%\n";
    std::cout << "Win Rate: " << m_winRate << "% (" << winningTrades << "/" << m_totalTrades << ")\n";
    std::cout << "Sharpe Ratio: " << m_sharpeRatio << "\n";
    std::cout << "Buy Signals: " << totalBuySignals << ", Sell Signals: " << totalSellSignals << "\n";
    std::cout << "Final Equity: $" << m_equityCurve.back() << " (Initial: $" << initialCapital << ")\n";
    std::cout << std::string(40, '-') << std::endl;
}

double Strategy::getTotalReturn() const {
    return m_totalReturn;
}

double Strategy::getAnnualReturn() const {
    return m_annualReturn;
}

double Strategy::getSharpeRatio() const {
    return m_sharpeRatio;
}

double Strategy::getMaxDrawdown() const {
    return m_maxDrawdown;
}

double Strategy::getWinRate() const {
    return m_winRate;
}

int Strategy::getTotalTrades() const {
    return m_totalTrades;
}

const std::vector<double>& Strategy::getEquityCurve() const {
    return m_equityCurve;
}

const std::vector<Trade>& Strategy::getTrades() const {
    return m_trades;
}

const std::string& Strategy::getName() const {
    return m_name;
}

} // namespace strategies
} // namespace crypto