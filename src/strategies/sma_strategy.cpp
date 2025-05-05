#include "strategies/sma_strategy.h"
#include <iostream>

namespace crypto {
namespace strategies {

SMAStrategy::SMAStrategy(int shortPeriod, int longPeriod) 
    : Strategy("SMA Crossover " + std::to_string(shortPeriod) + "/" + std::to_string(longPeriod)),
      m_shortPeriod(shortPeriod), m_longPeriod(longPeriod) {}

std::vector<Signal> SMAStrategy::generateSignals(const data::DataLoader& data) {
    const auto& priceData = data.getData();
    std::vector<Signal> signals(priceData.size(), HOLD);
    
    // Get SMAs
    std::vector<double> shortSMA = data.getSMA(m_shortPeriod);
    std::vector<double> longSMA = data.getSMA(m_longPeriod);
    
    // Check if indicators are available
    if (shortSMA.empty() || longSMA.empty()) {
        std::cerr << "Error: SMA indicators not available for " << m_name << ". Make sure they were calculated." << std::endl;
        return signals;
    }
    
    // Generate signals based on crossovers
    for (size_t i = 1; i < priceData.size(); ++i) {
        // Skip if not enough data for indicators
        if (i < static_cast<size_t>(m_longPeriod)) {
            continue;
        }
        
        // Buy signal: short SMA crosses above long SMA
        if (shortSMA[i] > longSMA[i] && shortSMA[i-1] <= longSMA[i-1]) {
            signals[i] = BUY;
        }
        // Sell signal: short SMA crosses below long SMA
        else if (shortSMA[i] < longSMA[i] && shortSMA[i-1] >= longSMA[i-1]) {
            signals[i] = SELL;
        }
    }
    
    return signals;
}

} // namespace strategies
} // namespace crypto