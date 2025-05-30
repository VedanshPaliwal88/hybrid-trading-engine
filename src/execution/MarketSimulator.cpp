#include "execution/MarketSimulator.h"

namespace trading {
    MarketSimulator::MarketSimulator() 
    : nextExecutionId_(1), rng_(std::random_device{}()), slippageDist_(-0.001, 0.001) {}
    
    void MarketSimulator::registerOrderBook(OrderBook* orderBook) {
        if (orderBook) {
            orderBooks_[orderBook->getSymbol()] = orderBook;
        }
    }

    bool MarketSimulator::submitOrder(const Order& order) {
        auto it = orderBooks_.find(order.symbol);
        if (it == orderBooks_.end()) {
            return false;
        }

        if (order.type == OrderType::MARKET) {
            processMarketOrder(order);
        } else {
            processLimitOrder(order);
        }

        return true;
    }

    void MarketSimulator::setExecutionCallback(std::function<void(const Execution&)> callback) {
        executionCallback_ = callback;
    }

    void MarketSimulator::processMarketOrder(const Order& order) {
        auto& orderBook = orderBooks_[order.symbol];

        double price;
        if (order.isBuy) {
            price = orderBook->getBestAsk();
        } else {
            price = orderBook->getBestBid();
        }

        price *= (1.0 + slippageDist_(rng_));

        Execution exec(generateExecutionId(), order.orderId, order.symbol, price, order.quantity);
    
        if (executionCallback_) {
            executionCallback_(exec);
        }
    }

    void MarketSimulator::processLimitOrder(const Order& order) {
        auto& orderBook = orderBooks_[order.symbol];

        bool canExecute = false;

        //checking if limit price is executable
        if (order.isBuy) {
            double bestAsk = orderBook->getBestAsk();
            canExecute = (bestAsk > 0.0 && order.price >= bestAsk);
        } else {
            double bestBid = orderBook->getBestBid();
            canExecute = (bestBid > 0.0 && order.price <= bestBid);
        }

        if (canExecute) {
            //get execution price (some slippage from best but clamped by order price)
            double price;
            if (order.isBuy) {
                price = std::min(order.price, orderBook->getBestAsk() * (1.0 + slippageDist_(rng_)));
            } else {
                price = std::max(order.price, orderBook->getBestBid() * (1.0 + slippageDist_(rng_)));
            }
            
            Execution exec(generateExecutionId(), order.orderId, order.symbol, price, order.quantity);

            if (executionCallback_) {
                executionCallback_(exec);
            }
        } else {
            //TODO: order waits in the order book 
        }
    }

    std::string MarketSimulator::generateExecutionId() {
        int id = nextExecutionId_++;
        return "EXECUTION-" + std::to_string(id);
    }
}