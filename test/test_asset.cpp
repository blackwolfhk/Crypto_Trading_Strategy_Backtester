#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "models/asset.h"
#include <vector>

TEST_CASE("Asset class functionality", "[asset]") {
    // Create test data
    std::vector<OHLCV> test_data = {
        {"2023-01-01", 100.0, 105.0, 95.0, 103.0, 1000.0},
        {"2023-01-02", 103.0, 108.0, 102.0, 107.0, 1200.0},
        {"2023-01-03", 107.0, 110.0, 105.0, 108.0, 1100.0},
        {"2023-01-04", 108.0, 112.0, 107.0, 110.0, 1300.0},
        {"2023-01-05", 110.0, 115.0, 109.0, 114.0, 1400.0}
    };
    
    Asset asset("TEST", test_data);
    
    SECTION("getClosePrices returns correct values") {
        auto prices = asset.getClosePrices();
        REQUIRE(prices.size() == 5);
        REQUIRE(prices[0] == 103.0);
        REQUIRE(prices[4] == 114.0);
    }
    
    SECTION("calculateReturns returns correct values") {
        auto returns = asset.calculateReturns();
        REQUIRE(returns.size() == 4);
        REQUIRE(returns[0] == Approx((107.0 / 103.0) - 1.0));
    }
    
    SECTION("calculateSMA returns correct values") {
        auto sma = asset.calculateSMA(3);
        REQUIRE(sma.size() == 3);
        REQUIRE(sma[0] == Approx((103.0 + 107.0 + 108.0) / 3.0));
    }
}