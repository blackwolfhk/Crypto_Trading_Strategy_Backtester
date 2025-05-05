#pragma once

#include "data/data_loader.h"
#include "strategies/strategy.h"
#include <memory>
#include <vector>
#include <string>

namespace crypto {
namespace backtester {

class Backtester {
public:
    Backtester(const std::string& dataPath);
    ~Backtester() = default;
    
    void addStrategy(std::shared_ptr<strategies::Strategy> strategy);
    void run(double initialCapital = 10000.0, double positionSize = 1.0);
    void compareStrategies() const;
    void exportResults(const std::string& outputDir = ".") const;

private:
    data::DataLoader m_dataLoader;
    std::vector<std::shared_ptr<strategies::Strategy>> m_strategies;
};

} // namespace backtester
} // namespace crypto