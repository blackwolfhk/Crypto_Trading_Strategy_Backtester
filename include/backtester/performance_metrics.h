#pragma once

#include <vector>
#include <string>

namespace crypto {
namespace backtester {

struct PerformanceMetrics {
    double totalReturn;
    double annualReturn;
    double sharpeRatio;
    double maxDrawdown;
    double winRate;
    int totalTrades;
    
    PerformanceMetrics() : 
        totalReturn(0.0), 
        annualReturn(0.0), 
        sharpeRatio(0.0), 
        maxDrawdown(0.0), 
        winRate(0.0), 
        totalTrades(0) {}
};

// Calculate performance metrics from equity curve
PerformanceMetrics calculateMetrics(
    const std::vector<double>& equityCurve, 
    const std::vector<std::pair<double, double>>& trades,
    double initialCapital,
    int daysInPeriod
);

// Calculate drawdown from equity curve
std::vector<double> calculateDrawdown(const std::vector<double>& equityCurve);

// Calculate Sharpe ratio
double calculateSharpeRatio(const std::vector<double>& returns, double riskFreeRate = 0.0);

} // namespace backtester
} // namespace crypto