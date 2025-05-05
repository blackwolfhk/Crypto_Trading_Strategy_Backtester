#pragma once
#include <string>
#include <vector>
#include "data/data_loader.h"

class Asset {
private:
    std::string symbol;
    std::vector<OHLCV> priceData;
    
public:
    Asset(const std::string& symbol, const std::vector<OHLCV>& data);
    
    std::vector<double> getClosePrices() const;
    std::vector<double> calculateReturns() const;
    double calculateVolatility(int period) const;
    std::vector<double> calculateSMA(int period) const;
    std::vector<double> calculateBollingerBands(int period, double stdDev, bool upper) const;
    
    const std::vector<OHLCV>& getPriceData() const;
    std::string getSymbol() const;
};