#pragma once

#include "data/data_loader.h"
#include <string>
#include <vector>

namespace crypto {
namespace strategies {

enum Signal {
    HOLD = 0,
    BUY = 1,
    SELL = -1
};

struct Trade {
    size_t entryIndex;
    size_t exitIndex;
    double entryPrice;
    double exitPrice;
    double profit;
    double profitPercent;
};

class Strategy {
public:
    Strategy(const std::string& name);
    virtual ~Strategy() = default;
    
    virtual std::vector<Signal> generateSignals(const data::DataLoader& data) = 0;
    virtual void backtest(const data::DataLoader& data, double initialCapital = 10000.0, double positionSize = 1.0);
    
    // Performance metrics
    double getTotalReturn() const;
    double getAnnualReturn() const;
    double getSharpeRatio() const;
    double getMaxDrawdown() const;
    double getWinRate() const;
    int getTotalTrades() const;
    
    // Get backtest results
    const std::vector<double>& getEquityCurve() const;
    const std::vector<Trade>& getTrades() const;
    const std::string& getName() const;

protected:
    std::string m_name;
    std::vector<Signal> m_signals;
    std::vector<double> m_equityCurve;
    std::vector<Trade> m_trades;
    
    // Performance metrics
    double m_totalReturn;
    double m_annualReturn;
    double m_sharpeRatio;
    double m_maxDrawdown;
    double m_winRate;
    int m_totalTrades;
};

} // namespace strategies
} // namespace crypto