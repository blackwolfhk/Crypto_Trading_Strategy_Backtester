#include "backtester/backtester.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

namespace crypto {
namespace backtester {

Backtester::Backtester(const std::string& dataPath) : m_dataLoader(dataPath) {
    if (!m_dataLoader.loadData()) {
        std::cerr << "Failed to load data from " << dataPath << ". Exiting..." << std::endl;
        exit(1);
    }
}

void Backtester::addStrategy(std::shared_ptr<strategies::Strategy> strategy) {
    m_strategies.push_back(strategy);
    std::cout << "Added strategy: " << strategy->getName() << std::endl;
}

void Backtester::run(double initialCapital, double positionSize) {
    std::cout << "\nPreparing indicators for backtesting..." << std::endl;
    
    // Calculate all indicators that might be needed by strategies
    m_dataLoader.addSMA(20);
    m_dataLoader.addSMA(50);
    m_dataLoader.addSMA(200);
    m_dataLoader.addEMA(12);
    m_dataLoader.addEMA(26);
    m_dataLoader.addRSI(14);
    m_dataLoader.addBollingerBands(20, 2.0);
    
    std::cout << "\nRunning backtests with initial capital: $" << initialCapital 
              << ", position size: " << (positionSize * 100) << "%" << std::endl;
    
    // Run backtest for each strategy
    for (auto& strategy : m_strategies) {
        strategy->backtest(m_dataLoader, initialCapital, positionSize);
    }
}

void Backtester::compareStrategies() const {
    if (m_strategies.empty()) {
        std::cerr << "No strategies to compare." << std::endl;
        return;
    }
    
    std::cout << "\n============= Strategy Comparison =============\n";
    
    // Header
    std::cout << std::left << std::setw(30) << "Strategy" 
              << std::right << std::setw(15) << "Total Return" 
              << std::setw(15) << "Annual Return" 
              << std::setw(15) << "Sharpe Ratio" 
              << std::setw(15) << "Max Drawdown" 
              << std::setw(15) << "Win Rate" 
              << std::setw(15) << "Total Trades" << std::endl;
    
    std::cout << std::string(105, '-') << std::endl;
    
    // Data
    for (const auto& strategy : m_strategies) {
        std::cout << std::left << std::setw(30) << strategy->getName() 
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2) << strategy->getTotalReturn() << "%" 
                  << std::setw(15) << strategy->getAnnualReturn() << "%" 
                  << std::setw(15) << std::setprecision(2) << strategy->getSharpeRatio() 
                  << std::setw(15) << std::setprecision(2) << strategy->getMaxDrawdown() << "%" 
                  << std::setw(15) << std::setprecision(2) << strategy->getWinRate() << "%" 
                  << std::setw(15) << strategy->getTotalTrades() << std::endl;
    }
}

void Backtester::exportResults(const std::string& outputDir) const {
    // Create strategy comparison CSV
    std::string comparisonFile = outputDir + "/strategy_comparison.csv";
    std::ofstream outfile(comparisonFile);
    
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing: " << comparisonFile << std::endl;
        return;
    }
    
    // Write header
    outfile << "Strategy,Total Return,Annual Return,Sharpe Ratio,Max Drawdown,Win Rate,Total Trades\n";
    
    // Write data for each strategy
    for (const auto& strategy : m_strategies) {
        outfile << strategy->getName() << "," 
                << strategy->getTotalReturn() << "," 
                << strategy->getAnnualReturn() << "," 
                << strategy->getSharpeRatio() << "," 
                << strategy->getMaxDrawdown() << "," 
                << strategy->getWinRate() << "," 
                << strategy->getTotalTrades() << "\n";
    }
    
    outfile.close();
    std::cout << "Strategy comparison exported to " << comparisonFile << std::endl;
    
    // Export individual strategy results
    const auto& priceData = m_dataLoader.getData();
    
    for (const auto& strategy : m_strategies) {
        // Create filename (replace spaces with underscores)
        std::string strategyName = strategy->getName();
        std::replace(strategyName.begin(), strategyName.end(), ' ', '_');
        std::string filename = outputDir + "/" + strategyName + ".csv";
        
        std::ofstream stratFile(filename);
        
        if (!stratFile.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            continue;
        }
        
        // Header
        stratFile << "Date,Close,Equity\n";
        
        const auto& equityCurve = strategy->getEquityCurve();
        
        // Data
        for (size_t i = 0; i < equityCurve.size(); ++i) {
            stratFile << priceData[i].date << "," 
                    << priceData[i].close << "," 
                    << equityCurve[i] << "\n";
        }
        
        stratFile.close();
        std::cout << "Results for " << strategy->getName() << " exported to " << filename << std::endl;
    }
}

} // namespace backtester
} // namespace crypto