#include "mean_reversion.h"

MeanReversionStrategy::MeanReversionStrategy(int period, double std)
    : sma_period(period), entry_std(std) {}

std::vector<Signal> MeanReversionStrategy::generateSignals(const Asset& asset) {
    std::vector<Signal> signals;
    auto prices = asset.getClosePrices();
    
    if (prices.size() < static_cast<size_t>(sma_period)) {
        return signals;
    }
    
    auto sma = asset.calculateSMA(sma_period);
    auto upper_band = asset.calculateBollingerBands(sma_period, entry_std, true);
    auto lower_band = asset.calculateBollingerBands(sma_period, entry_std, false);
    
    // Fill initial signals with HOLD
    signals.resize(sma_period - 1, HOLD);
    
    for (size_t i = 0; i < sma.size(); i++) {
        size_t price_idx = i + sma_period - 1;
        
        if (prices[price_idx] > upper_band[i]) {
            signals.push_back(SELL);  // Price above upper band - sell signal
        } else if (prices[price_idx] < lower_band[i]) {
            signals.push_back(BUY);   // Price below lower band - buy signal
        } else {
            signals.push_back(HOLD);  // Price within bands - no signal
        }
    }
    
    return signals;
}

std::string MeanReversionStrategy::getName() const {
    return "Mean Reversion (Bollinger Bands) Strategy";
}