#pragma once
#include "models/asset.h"
#include <vector>

enum Signal {
    SELL = -1,
    HOLD = 0,
    BUY = 1
};

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual std::vector<Signal> generateSignals(const Asset& asset) = 0;
    virtual std::string getName() const = 0;
};