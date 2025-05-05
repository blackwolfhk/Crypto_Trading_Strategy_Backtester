#pragma once

#include "strategies/strategy.h"

namespace crypto {
namespace strategies {

class RSIStrategy : public Strategy {
public:
    RSIStrategy(int period, double oversold, double overbought);
    ~RSIStrategy() = default;
    
    std::vector<Signal> generateSignals(const data::DataLoader& data) override;

private:
    int m_period;
    double m_oversold;
    double m_overbought;
};

} // namespace strategies
} // namespace crypto