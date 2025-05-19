#pragma once

#include "market_data/MarketTypes.h"
#include <map>
#include <string>

namespace trading {
    class OrderBook {
        public:
        OrderBook(const std::string& symbol);

        void update(const MarketUpdate& update);

        double getBestBid() const;
        double getBestAsk() const;

        double getSpread() const;

        int getVolumeAtPrice(double price, Side side) const;

        void print() const;

        private:
        std::string symbol_;
        std::map<double, int, std::greater<double>> bids_;
        std::map<double, int> asks_;
    };
}