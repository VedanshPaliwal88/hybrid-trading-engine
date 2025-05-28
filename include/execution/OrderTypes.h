#pragma once

#include <string>
#include <chrono>

namespace trading {
    enum class OrderType {
        MARKET,
        LIMIT
    };

    enum class OrderStatus {
        NEW,
        PARTIALLY_FILLED,
        FILLED,
        CANCELLED,
        REJECTED
    };

    struct Order {
        std::string orderId;
        std::string symbol;
        bool isBuy;
        double price;
        int quantity;
        int filledQuantity;
        OrderType type;
        OrderStatus status;
        std::chrono::nanoseconds timestamp;
    
        Order(const std::string& id, const std::string& sym, bool buy, double p, int qty, OrderType t)
        : orderId(id), symbol(sym), isBuy(buy), price(p), quantity(qty), filledQuantity(0), 
        type(t), status(OrderStatus::NEW), timestamp(std::chrono::high_resolution_clock::now().time_since_epoch()) {}
    };

    struct Execution {
        std::string executionId;
        std::string orderId;
        std::string symbol;
        double price;
        int quantity;
        std::chrono::nanoseconds timestamp;

        Execution(const std::string& execId, const std::string& id, const std::string& sym,
        double p, int qty)
        : executionId(execId), orderId(id), symbol(sym), price(p), quantity(qty),
        timestamp(std::chrono::high_resolution_clock::now().time_since_epoch()) {}
    };
};