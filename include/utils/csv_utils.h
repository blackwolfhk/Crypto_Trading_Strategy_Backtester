#pragma once

#include <string>
#include <vector>
#include <map>

namespace crypto {
namespace utils {

// Read CSV file
std::vector<std::vector<std::string>> readCSV(const std::string& filename, char delimiter = ',', bool skipHeader = true);

// Write data to CSV file
bool writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter = ',');

// Write map to CSV file
template<typename KeyType, typename ValueType>
bool writeMapToCSV(const std::string& filename, const std::map<KeyType, ValueType>& data, const std::string& keyHeader, const std::string& valueHeader);

} // namespace utils
} // namespace crypto