#include "data/data_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace crypto {
namespace data {

DataLoader::DataLoader(const std::string& filePath) : m_filePath(filePath) {}

bool DataLoader::loadData() {
    std::ifstream file(m_filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << m_filePath << std::endl;
        return false;
    }
    
    std::string line;
    
    // Skip first two lines (URL and header)
    std::getline(file, line);
    std::getline(file, line);
    
    // Process each line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        
        OHLCV data;
        
        // Parse each column (unix,date,symbol,open,high,low,close,Volume BTC,Volume USD)
        std::getline(ss, token, ',');
        data.unix_time = std::stol(token);
        
        std::getline(ss, token, ',');
        data.date = token;
        
        std::getline(ss, token, ',');
        data.symbol = token;
        
        std::getline(ss, token, ',');
        data.open = std::stod(token);
        
        std::getline(ss, token, ',');
        data.high = std::stod(token);
        
        std::getline(ss, token, ',');
        data.low = std::stod(token);
        
        std::getline(ss, token, ',');
        data.close = std::stod(token);
        
        std::getline(ss, token, ',');
        data.volume_btc = std::stod(token);
        
        std::getline(ss, token, ',');
        if (!token.empty()) {
            data.volume_usd = std::stod(token);
        }
        
        m_data.push_back(data);
    }
    
    // If data is in descending order (newest first), reverse it
    if (!m_data.empty() && !m_data.back().date.empty() && !m_data.front().date.empty()) {
        if (m_data.front().date > m_data.back().date) {
            std::reverse(m_data.begin(), m_data.end());
        }
    }
    
    std::cout << "Loaded " << m_data.size() << " records from " << m_filePath << std::endl;
    
    if (!m_data.empty()) {
        std::cout << "Data range: " << m_data.front().date << " to " << m_data.back().date << std::endl;
    }
    
    return !m_data.empty();
}

const std::vector<OHLCV>& DataLoader::getData() const {
    return m_data;
}

std::pair<std::string, std::string> DataLoader::getDateRange() const {
    if (m_data.empty()) {
        return {"", ""};
    }
    return {m_data.front().date, m_data.back().date};
}

void DataLoader::addSMA(int period) {
    if (m_data.empty() || period <= 0 || period > static_cast<int>(m_data.size())) {
        return;
    }
    
    // Check if already calculated
    if (m_sma.find(period) != m_sma.end()) {
        return;
    }
    
    std::vector<double> sma(m_data.size(), 0.0);
    
    // Calculate SMA
    for (size_t i = 0; i < m_data.size(); ++i) {
        if (i < static_cast<size_t>(period - 1)) {
            sma[i] = 0.0;  // Not enough data
        } else {
            double sum = 0.0;
            for (int j = 0; j < period; ++j) {
                sum += m_data[i - j].close;
            }
            sma[i] = sum / period;
        }
    }
    
    m_sma[period] = sma;
    std::cout << "Calculated SMA(" << period << ")" << std::endl;
}

void DataLoader::addEMA(int period) {
    if (m_data.empty() || period <= 0 || period > static_cast<int>(m_data.size())) {
        return;
    }
    
    // Check if already calculated
    if (m_ema.find(period) != m_ema.end()) {
        return;
    }
    
    std::vector<double> ema(m_data.size(), 0.0);
    
    // First value is SMA
    double sum = 0.0;
    for (int i = 0; i < period; ++i) {
        sum += m_data[i].close;
    }
    ema[period - 1] = sum / period;
    
    // Multiplier: (2 / (Time periods + 1) )
    double multiplier = 2.0 / (period + 1.0);
    
    // Calculate EMA for the rest
    for (size_t i = period; i < m_data.size(); ++i) {
        ema[i] = (m_data[i].close - ema[i - 1]) * multiplier + ema[i - 1];
    }
    
    m_ema[period] = ema;
    std::cout << "Calculated EMA(" << period << ")" << std::endl;
}

void DataLoader::addRSI(int period) {
    if (m_data.empty() || period <= 0 || period > static_cast<int>(m_data.size())) {
        return;
    }
    
    // Check if already calculated
    if (m_rsi.find(period) != m_rsi.end()) {
        return;
    }
    
    std::vector<double> rsi(m_data.size(), 0.0);
    std::vector<double> gains(m_data.size(), 0.0);
    std::vector<double> losses(m_data.size(), 0.0);
    
    // Calculate price changes
    for (size_t i = 1; i < m_data.size(); ++i) {
        double change = m_data[i].close - m_data[i - 1].close;
        if (change > 0) {
            gains[i] = change;
            losses[i] = 0.0;
        } else {
            gains[i] = 0.0;
            losses[i] = -change;
        }
    }
    
    // First RSI calculation uses simple average
    double avgGain = 0.0;
    double avgLoss = 0.0;
    
    for (int i = 1; i <= period; ++i) {
        avgGain += gains[i];
        avgLoss += losses[i];
    }
    
    avgGain /= period;
    avgLoss /= period;
    
    // Calculate first RSI
    double rs = (avgLoss == 0) ? 100.0 : avgGain / avgLoss;
    rsi[period] = 100.0 - (100.0 / (1.0 + rs));
    
    // Calculate remaining RSI values using smoothed average
    for (size_t i = period + 1; i < m_data.size(); ++i) {
        avgGain = (avgGain * (period - 1) + gains[i]) / period;
        avgLoss = (avgLoss * (period - 1) + losses[i]) / period;
        
        rs = (avgLoss == 0) ? 100.0 : avgGain / avgLoss;
        rsi[i] = 100.0 - (100.0 / (1.0 + rs));
    }
    
    m_rsi[period] = rsi;
    std::cout << "Calculated RSI(" << period << ")" << std::endl;
}

void DataLoader::addBollingerBands(int period, double stdDev) {
    if (m_data.empty() || period <= 0 || period > static_cast<int>(m_data.size())) {
        return;
    }
    
    // Calculate SMA first (middle band)
    addSMA(period);
    
    // Check if already calculated
    if (m_bollingerUpper.find(period) != m_bollingerUpper.end()) {
        return;
    }
    
    std::vector<double> upper(m_data.size(), 0.0);
    std::vector<double> lower(m_data.size(), 0.0);
    
    // Calculate standard deviation and bands
    for (size_t i = period - 1; i < m_data.size(); ++i) {
        double sum = 0.0;
        for (int j = 0; j < period; ++j) {
            double diff = m_data[i - j].close - m_sma[period][i];
            sum += diff * diff;
        }
        double stdDev_val = std::sqrt(sum / period);
        
        upper[i] = m_sma[period][i] + stdDev * stdDev_val;
        lower[i] = m_sma[period][i] - stdDev * stdDev_val;
    }
    
    m_bollingerUpper[period] = upper;
    m_bollingerLower[period] = lower;
    
    std::cout << "Calculated Bollinger Bands(" << period << ", " << stdDev << ")" << std::endl;
}

std::vector<double> DataLoader::getSMA(int period) const {
    auto it = m_sma.find(period);
    if (it != m_sma.end()) {
        return it->second;
    }
    return std::vector<double>();
}

std::vector<double> DataLoader::getEMA(int period) const {
    auto it = m_ema.find(period);
    if (it != m_ema.end()) {
        return it->second;
    }
    return std::vector<double>();
}

std::vector<double> DataLoader::getRSI(int period) const {
    auto it = m_rsi.find(period);
    if (it != m_rsi.end()) {
        return it->second;
    }
    return std::vector<double>();
}

std::vector<double> DataLoader::getBollingerUpper(int period) const {
    auto it = m_bollingerUpper.find(period);
    if (it != m_bollingerUpper.end()) {
        return it->second;
    }
    return std::vector<double>();
}

std::vector<double> DataLoader::getBollingerLower(int period) const {
    auto it = m_bollingerLower.find(period);
    if (it != m_bollingerLower.end()) {
        return it->second;
    }
    return std::vector<double>();
}

} // namespace data
} // namespace crypto