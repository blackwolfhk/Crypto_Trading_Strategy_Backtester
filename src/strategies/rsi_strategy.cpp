#include "strategies/rsi_strategy.h"
#include <iostream>

namespace crypto {
namespace strategies {

RSIStrategy::RSIStrategy(int period, double oversold, double overbought) 
    : Strategy("RSI " + std::to_string(period) + " (" + std::to_string(static_cast<int>(oversold)) + 
                "/" + std::to_string(static_cast<int>(overbought)) + ")"),
      m_period(period), m_oversold(oversold), m_overbought(overbought) {}

std::vector<Signal> RSIStrategy::generateSignals(const data::DataLoader& data) {
    const auto& priceData = data.getData();
    std::vector<Signal> signals(priceData.size(), HOLD);
    
    // Get RSI
    std::vector<double> rsi = data.getRSI(m_period);
    
    // Check if indicator is available
    if (rsi.empty()) {
        std::cerr << "Error: RSI indicator not available for " << m_name << ". Make sure it was calculated." << std::endl;
        return signals;
    }
    
    // Generate signals based on RSI levels
    for (size_t i = 1; i < priceData.size(); ++i) {
        // Skip if not enough data for indicator
        if (i < static_cast<size_t>(m_period + 1)) {
            continue;
        }
        
        // Buy signal: RSI crosses above oversold level
        if (rsi[i] > m_oversold && rsi[i-1] <= m_oversold) {
            signals[i] = BUY;
        }
        // Sell signal: RSI crosses below overbought level
        else if (rsi[i] < m_overbought && rsi[i-1] >= m_overbought) {
            signals[i] = SELL;
        }
    }
    
    return signals;
}

} // namespace strategies
} // namespace crypto