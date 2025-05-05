#include "strategies/bollinger_bands_strategy.h"
#include <iostream>

namespace crypto {
namespace strategies {

BollingerBandsStrategy::BollingerBandsStrategy(int period, double stdDev) 
    : Strategy("Bollinger Bands " + std::to_string(period) + " (" + std::to_string(stdDev) + ")"),
      m_period(period), m_stdDev(stdDev) {}

std::vector<Signal> BollingerBandsStrategy::generateSignals(const data::DataLoader& data) {
    const auto& priceData = data.getData();
    std::vector<Signal> signals(priceData.size(), HOLD);
    
    // Get Bollinger Bands
    std::vector<double> upperBand = data.getBollingerUpper(m_period);
    std::vector<double> lowerBand = data.getBollingerLower(m_period);
    
    // Check if indicators are available
    if (upperBand.empty() || lowerBand.empty()) {
        std::cerr << "Error: Bollinger Band indicators not available for " << m_name 
                  << ". Make sure they were calculated." << std::endl;
        return signals;
    }
    
    // Generate signals based on price touching bands
    for (size_t i = 1; i < priceData.size(); ++i) {
        // Skip if not enough data for indicators
        if (i < static_cast<size_t>(m_period)) {
            continue;
        }
        
        // Buy signal: price crosses above lower band
        if (priceData[i].close > lowerBand[i] && priceData[i-1].close <= lowerBand[i-1]) {
            signals[i] = BUY;
        }
        // Sell signal: price crosses below upper band
        else if (priceData[i].close < upperBand[i] && priceData[i-1].close >= upperBand[i-1]) {
            signals[i] = SELL;
        }
    }
    
    return signals;
}

} // namespace strategies
} // namespace crypto