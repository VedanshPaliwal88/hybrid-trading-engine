#pragma once

#include "execution/OrderTypes.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <atomic>

namespace trading {
    class OrderManager {
        public:
        OrderManager();

        std::string submitOrder(const std::string& symbol, bool isBuy, double price, int quantity, OrderType type);

        bool cancelOrder(const std::string& orderId);

        const Order* getOrder(const std::string& orderId) const;

        void setOrderStatusCallback(std::function<void(const Order&)> callback);

        void setExecutionCallback(std::function<void(const Execution&)> callback);

        void processExecution(const Execution& execution);

        private:
        std::unordered_map<std::string, Order> orders_;
        std::function<void(const Order&)> statusCallback_;
        std::function<void(const Execution&)> executionCallback_;
        std::atomic<int> nextOrderId_;

        std::string generateOrderId();
    };
};