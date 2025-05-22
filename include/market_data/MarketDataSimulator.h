#pragma once

#include "market_data/MarketTypes.h"
#include <string>
#include <vector>
#include <random>
#include <functional>

namespace trading {
    class MarketDataSimulator {
        public:
        MarketDataSimulator(const std::string& symbol, double startPrice, double volatility = 0.001);

        MarketUpdate generateUpdate();

        void setUpdateCallback(std::function<void(const MarketUpdate&)> callback);

        void start(int updateFrequencyMs);

        void stop();


        private:
        std::string symbol_;
        double currentPrice_;
        double volatility_;
        std::mt19937 rng_;
        std::normal_distribution<double> priceChange_;
        std::uniform_int_distribution<int> quantityDist_;
        std::uniform_int_distribution<int> sideDist_;
        std::function<void(const MarketUpdate&)> callback_;
        bool running_;

    };
}