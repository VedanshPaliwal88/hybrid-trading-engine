/*
Contains the basic market data structures
*/

#pragma once

#include <string>
#include <chrono>

namespace trading {
    enum class Side {
        BID,
        ASK
    };

    struct MarketUpdate {
        std::string symbol;
        double price;
        int quantity;
        Side side;
        std::chrono::nanoseconds timestamp;

        MarketUpdate (const std::string& sym, double p, int q, Side s) 
        : symbol(sym), price(p), quantity(q), side(s), 
        timestamp(std::chrono::high_resolution_clock::now().time_since_epoch()) {}     
    };

    struct PriceLevel {
        double price;
        int quantity;
    };
}