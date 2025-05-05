#include "data_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<OHLCV> DataLoader::loadCSV(const std::string& filename) {
    std::vector<OHLCV> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        OHLCV candle;
        
        // timestamp
        std::getline(ss, value, ',');
        candle.timestamp = value;
        
        // open
        std::getline(ss, value, ',');
        candle.open = std::stod(value);
        
        // high
        std::getline(ss, value, ',');
        candle.high = std::stod(value);
        
        // low
        std::getline(ss, value, ',');
        candle.low = std::stod(value);
        
        // close
        std::getline(ss, value, ',');
        candle.close = std::stod(value);
        
        // volume
        std::getline(ss, value, ',');
        candle.volume = std::stod(value);
        
        data.push_back(candle);
    }
    
    return data;
}