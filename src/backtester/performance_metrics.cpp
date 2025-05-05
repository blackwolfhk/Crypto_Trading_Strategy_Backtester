#include "backtester/performance_metrics.h"
#include <cmath>
#include <numeric>
#include <algorithm>

namespace crypto {
namespace backtester {

PerformanceMetrics calculateMetrics(
    const std::vector<double>& equityCurve, 
    const std::vector<std::pair<double, double>>& trades,
    double initialCapital,
    int daysInPeriod
) {
    PerformanceMetrics metrics;
    
    if (equityCurve.empty()) {
        return metrics;
    }
    
    // Calculate total return
    metrics.totalReturn = (equityCurve.back() / initialCapital - 1.0) * 100.0;
    
    // Calculate annualized return
    double years = static_cast<double>(equityCurve.size()) / daysInPeriod;
    metrics.annualReturn = (std::pow(1.0 + metrics.totalReturn / 100.0, 1.0 / years) - 1.0) * 100.0;
    
    // Calculate max drawdown
    std::vector<double> drawdowns = calculateDrawdown(equityCurve);
    metrics.maxDrawdown = drawdowns.empty() ? 0.0 : *std::min_element(drawdowns.begin(), drawdowns.end()) * -1.0;
    
    // Calculate win rate
    metrics.totalTrades = static_cast<int>(trades.size());
    int winningTrades = 0;
    
    for (const auto& trade : trades) {
        if (trade.second > trade.first) {
            winningTrades++;
        }
    }
    
    metrics.winRate = metrics.totalTrades > 0 ? 
                    static_cast<double>(winningTrades) / metrics.totalTrades * 100.0 : 0.0;
    
    // Calculate daily returns for Sharpe ratio
    std::vector<double> returns;
    for (size_t i = 1; i < equityCurve.size(); ++i) {
        returns.push_back(equityCurve[i] / equityCurve[i-1] - 1.0);
    }
    
    metrics.sharpeRatio = calculateSharpeRatio(returns);
    
    return metrics;
}

std::vector<double> calculateDrawdown(const std::vector<double>& equityCurve) {
    std::vector<double> drawdowns(equityCurve.size(), 0.0);
    
    if (equityCurve.empty()) {
        return drawdowns;
    }
    
    double peak = equityCurve[0];
    
    for (size_t i = 0; i < equityCurve.size(); ++i) {
        if (equityCurve[i] > peak) {
            peak = equityCurve[i];
        }
        
        drawdowns[i] = (equityCurve[i] - peak) / peak * 100.0;
    }
    
    return drawdowns;
}

double calculateSharpeRatio(const std::vector<double>& returns, double riskFreeRate) {
    if (returns.empty()) {
        return 0.0;
    }
    
    double meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    
    double variance = 0.0;
    for (const auto& ret : returns) {
        variance += (ret - meanReturn) * (ret - meanReturn);
    }
    variance /= returns.size();
    
    double stdDev = std::sqrt(variance);
    
    if (stdDev == 0.0) {
        return 0.0;
    }
    
    // Annualize Sharpe ratio (assuming 252 trading days per year)
    return (meanReturn - riskFreeRate) / stdDev * std::sqrt(252.0);
}

} // namespace backtester
} // namespace crypto