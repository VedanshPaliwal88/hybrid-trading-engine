#include "market_data/MarketDataSimulator.h"
#include <thread>
#include <chrono>

namespace trading
{
    MarketDataSimulator::MarketDataSimulator(const std::string& symbol, double startPrice, double volatility) :
    symbol_(symbol), currentPrice_(startPrice), volatility_(volatility),
    rng_(std::random_device{}()),
    priceChange_(0.0, volatility),
    quantityDist_(1, 100),
    sideDist_(0, 1),
    running_(false) {}

    MarketUpdate MarketDataSimulator::generateUpdate() {
        double change = priceChange_(rng_);
        currentPrice_ = std::max(0.01, currentPrice_ * (1.0 + change));

        Side side = sideDist_(rng_) == 0 ? Side::BID : Side::ASK;

        double price = currentPrice_;
        //generating spread
        if (side == Side::BID) {
            price *= 0.999;
        } else {
            price *= 1.001;
        }

        int quantity = quantityDist_(rng_);

        return MarketUpdate(symbol_, price, quantity, side);
    }

    void MarketDataSimulator::setUpdateCallback(std::function<void(const MarketUpdate&)> callback) {
        callback_ = callback;
    }

    void MarketDataSimulator::stop() {
        running_ = false;
    }

    void MarketDataSimulator::start(int updateFrequencyMs) {
        if (running_) {
            return;
        }

        running_ = true;

        std::thread([this, updateFrequencyMs]() {
            while (running_) {
                MarketUpdate update = generateUpdate();

                if (callback_) {
                    callback_(update);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(updateFrequencyMs));
            }
        }).detach();
    }
} 