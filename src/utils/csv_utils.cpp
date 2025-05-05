#include "utils/csv_utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace crypto {
namespace utils {

std::vector<std::vector<std::string>> readCSV(const std::string& filename, char delimiter, bool skipHeader) {
    std::vector<std::vector<std::string>> result;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return result;
    }
    
    std::string line;
    
    // Skip header if requested
    if (skipHeader && std::getline(file, line)) {
        // Header skipped
    }
    
    // Read data
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        
        while (std::getline(ss, cell, delimiter)) {
            row.push_back(cell);
        }
        
        result.push_back(row);
    }
    
    return result;
}

bool writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing " << filename << std::endl;
        return false;
    }
    
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << delimiter;
            }
        }
        file << "\n";
    }
    
    return true;
}

template<typename KeyType, typename ValueType>
bool writeMapToCSV(const std::string& filename, const std::map<KeyType, ValueType>& data, 
                   const std::string& keyHeader, const std::string& valueHeader) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing " << filename << std::endl;
        return false;
    }
    
    // Write header
    file << keyHeader << "," << valueHeader << "\n";
    
    // Write data
    for (const auto& [key, value] : data) {
        file << key << "," << value << "\n";
    }
    
    return true;
}

// Explicit template instantiations for common types
template bool writeMapToCSV<std::string, double>(
    const std::string&, const std::map<std::string, double>&, 
    const std::string&, const std::string&);

template bool writeMapToCSV<std::string, int>(
    const std::string&, const std::map<std::string, int>&, 
    const std::string&, const std::string&);

template bool writeMapToCSV<int, double>(
    const std::string&, const std::map<int, double>&, 
    const std::string&, const std::string&);

} // namespace utils
} // namespace crypto