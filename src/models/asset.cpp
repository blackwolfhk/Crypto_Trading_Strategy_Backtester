#include "asset.h"
#include <cmath>
#include <numeric>
#include <algorithm>

Asset::Asset(const std::string& sym, const std::vector<OHLCV>& data) 
    : symbol(sym), priceData(data) {}

std::vector<double> Asset::getClosePrices() const {
    std::vector<double> prices;
    prices.reserve(priceData.size());
    
    for (const auto& candle : priceData) {
        prices.push_back(candle.close);
    }
    
    return prices;
}

std::vector<double> Asset::calculateReturns() const {
    std::vector<double> returns;
    auto prices = getClosePrices();
    
    if (prices.size() < 2) {
        return returns;
    }
    
    returns.reserve(prices.size() - 1);
    
    for (size_t i = 1; i < prices.size(); i++) {
        double ret = (prices[i] / prices[i-1]) - 1.0;
        returns.push_back(ret);
    }
    
    return returns;
}

double Asset::calculateVolatility(int period) const {
    auto returns = calculateReturns();
    
    if (returns.empty() || period > static_cast<int>(returns.size())) {
        return 0.0;
    }
    
    // Use the last 'period' returns
    auto start = returns.end() - std::min(period, static_cast<int>(returns.size()));
    auto end = returns.end();
    
    double sum = std::accumulate(start, end, 0.0);
    double mean = sum / period;
    
    double sq_sum = std::inner_product(start, end, start, 0.0,
                                     std::plus<>(),
                                     [mean](double a, double b) {
                                         return (a - mean) * (b - mean);
                                     });
    
    return std::sqrt(sq_sum / period);
}

std::vector<double> Asset::calculateSMA(int period) const {
    std::vector<double> sma;
    auto prices = getClosePrices();
    
    if (prices.size() < static_cast<size_t>(period)) {
        return sma;
    }
    
    sma.reserve(prices.size() - period + 1);
    
    for (size_t i = period - 1; i < prices.size(); i++) {
        double sum = 0.0;
        for (size_t j = 0; j < period; j++) {
            sum += prices[i - j];
        }
        sma.push_back(sum / period);
    }
    
    return sma;
}

std::vector<double> Asset::calculateBollingerBands(int period, double stdDev, bool upper) const {
    std::vector<double> bands;
    auto prices = getClosePrices();
    auto sma = calculateSMA(period);
    
    if (sma.empty()) {
        return bands;
    }
    
    bands.reserve(sma.size());
    
    for (size_t i = 0; i < sma.size(); i++) {
        size_t start_idx = i;
        size_t end_idx = i + period;
        
        double sum_sq_diff = 0.0;
        for (size_t j = start_idx; j < end_idx; j++) {
            double diff = prices[j] - sma[i];
            sum_sq_diff += diff * diff;
        }
        
        double std_deviation = std::sqrt(sum_sq_diff / period);
        
        if (upper) {
            bands.push_back(sma[i] + stdDev * std_deviation);
        } else {
            bands.push_back(sma[i] - stdDev * std_deviation);
        }
    }
    
    return bands;
}

const std::vector<OHLCV>& Asset::getPriceData() const {
    return priceData;
}

std::string Asset::getSymbol() const {
    return symbol;
}