#pragma once
#include "strategy.h"
#include <string>

class MeanReversionStrategy : public Strategy {
private:
    int sma_period;
    double entry_std;
    
public:
    MeanReversionStrategy(int period = 20, double std = 2.0);
    std::vector<Signal> generateSignals(const Asset& asset) override;
    std::string getName() const override;
};