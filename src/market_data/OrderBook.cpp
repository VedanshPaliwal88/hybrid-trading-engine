#include "market_data/OrderBook.h"
#include <iostream>
#include <iomanip>

namespace trading {
    OrderBook::OrderBook(const std::string& symbol) : symbol_(symbol) {}

    void OrderBook::update(const MarketUpdate& update) {
        //only track own updates
        if (update.symbol != symbol_) {
            return;
        }

        double price = update.price;
        int quantity = update.quantity;

        if (update.side == Side::BID) {
            if (quantity > 0) {
                bids_[price] = quantity;
            } else {
                bids_.erase(price);
            }
        } else {
            if (quantity > 0) {
                asks_[price] = quantity;
            } else {
                asks_.erase(price);
            }
        }
    }

    double OrderBook::getBestBid() const {
        if (bids_.empty()) {
            return 0.0;
        }
        return bids_.begin() -> first;
    }

    double OrderBook::getBestAsk() const {
        if (asks_.empty()) {
            return 0.0;
        }
        return asks_.begin()->first;
    }

    double OrderBook::getSpread() const {
        double bestBid = getBestBid();
        double bestAsk = getBestAsk();

        if (bestBid == 0.0 || bestAsk == 0.0) {
            return 0.0;
        }

        return bestAsk - bestBid;
    }

    int OrderBook::getVolumeAtPrice(double price, Side side) const {
        if (side == Side::BID) {
            auto it = asks_.find(price);
            return (it != asks_.end()) ? it->second : 0;
        } else {
            auto it = bids_.find(price);
            return (it != bids_.end()) ? it->second : 0;
        }
    }

    void OrderBook::print() const {
        std::cout << "Order book for: " << symbol_ << std::endl;
        std::cout << std::setw(10) << "BID SIZE" << " | " 
                    << std::setw(10) << "BID PRICE" << " | "
                    << std::setw(10) << "ASK SIZE" << " | "
                    << std::setw(10) << "ASK PRICE" << " | " << std::endl;
        std::cout << std::string(46, '-') << std::endl;

        auto bidIt = bids_.begin();
        auto askIt = asks_.begin();

        for (int i = 0; i < 5; i++) {
            std::cout << std::setw(10);
            if (bidIt != bids_.end()) {
                std::cout << bidIt->second << " | " << std::setw(10) << std::fixed << std::setprecision(2) << bidIt->first;
                ++bidIt;
            } else {
                std::cout << " " << " | " << std::setw(10) << " ";
            }
            
            std::cout << " | " << std::setw(10);

            if (askIt != asks_.end()) {
                std::cout << askIt->second << " | " << std::setw(10) << std::fixed << std::setprecision(2) << askIt->first;
                ++askIt;
            } else {
                std::cout << " " << " | " << std::setw(10) << " ";
            }

            std::cout << std::endl;
        }
    }
}