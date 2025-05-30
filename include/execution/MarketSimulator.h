#pragma once

#include "market_data/OrderBook.h"
#include "execution/OrderTypes.h"
#include <unordered_map>
#include <functional>
#include <atomic>
#include <random>

namespace trading {
    class MarketSimulator {
        public:
        MarketSimulator();

        void registerOrderBook(OrderBook* orderBook);

        bool submitOrder(const Order& order);

        void setExecutionCallback(std::function<void(const Execution&)> callback);

        private:
        std::unordered_map<std::string, OrderBook*> orderBooks_;
        std::function<void(const Execution&)> executionCallback_;
        std::atomic<int> nextExecutionId_;
        std::mt19937 rng_;
        std::uniform_real_distribution<double> slippageDist_;

        void processMarketOrder(const Order& order);
        
        void processLimitOrder(const Order& order);

        std::string generateExecutionId();
    };
}