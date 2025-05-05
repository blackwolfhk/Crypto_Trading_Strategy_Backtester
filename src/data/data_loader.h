#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct OHLCV {
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class DataLoader {
public:
    std::vector<OHLCV> loadCSV(const std::string& filename);
};