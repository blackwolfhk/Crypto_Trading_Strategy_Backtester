#pragma once

#include "strategies/strategy.h"

namespace crypto {
namespace strategies {

class SMAStrategy : public Strategy {
public:
    SMAStrategy(int shortPeriod, int longPeriod);
    ~SMAStrategy() = default;
    
    std::vector<Signal> generateSignals(const data::DataLoader& data) override;

private:
    int m_shortPeriod;
    int m_longPeriod;
};

} // namespace strategies
} // namespace crypto