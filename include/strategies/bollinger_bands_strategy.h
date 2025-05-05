#pragma once

#include "strategies/strategy.h"

namespace crypto {
namespace strategies {

class BollingerBandsStrategy : public Strategy {
public:
    BollingerBandsStrategy(int period, double stdDev);
    ~BollingerBandsStrategy() = default;
    
    std::vector<Signal> generateSignals(const data::DataLoader& data) override;

private:
    int m_period;
    double m_stdDev;
};

} // namespace strategies
} // namespace crypto