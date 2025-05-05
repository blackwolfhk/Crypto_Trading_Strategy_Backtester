#pragma once

#include <string>
#include <vector>
#include <map>

namespace crypto {
namespace data {

// Structure to hold OHLCV data
struct OHLCV {
    long unix_time;
    std::string date;
    std::string symbol;
    double open;
    double high;
    double low;
    double close;
    double volume_btc;
    double volume_usd;
};

class DataLoader {
public:
    DataLoader(const std::string& filePath);
    ~DataLoader() = default;
    
    bool loadData();
    const std::vector<OHLCV>& getData() const;
    std::pair<std::string, std::string> getDateRange() const;
    
    // Technical indicators
    void addSMA(int period);
    void addEMA(int period);
    void addRSI(int period);
    void addBollingerBands(int period, double stdDev);
    
    // Get indicators
    std::vector<double> getSMA(int period) const;
    std::vector<double> getEMA(int period) const;
    std::vector<double> getRSI(int period) const;
    std::vector<double> getBollingerUpper(int period) const;
    std::vector<double> getBollingerLower(int period) const;

private:
    std::string m_filePath;
    std::vector<OHLCV> m_data;
    
    // Store calculated indicators
    std::map<int, std::vector<double>> m_sma;
    std::map<int, std::vector<double>> m_ema;
    std::map<int, std::vector<double>> m_rsi;
    std::map<int, std::vector<double>> m_bollingerUpper;
    std::map<int, std::vector<double>> m_bollingerLower;
};

} // namespace data
} // namespace crypto