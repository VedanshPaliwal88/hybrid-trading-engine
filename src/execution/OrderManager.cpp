#include "execution/OrderManager.h"

namespace trading {
    OrderManager::OrderManager() : nextOrderId_(1) {}

    std::string OrderManager::submitOrder(const std::string& symbol, bool isBuy, double price, int quantity, OrderType type) {
        std::string orderId = generateOrderId();

        Order newOrder(orderId, symbol, isBuy, price, quantity, type);
        orders_[orderId] = newOrder;

        if (statusCallback_) {
            statusCallback_(newOrder);
        }
    
        return orderId;
    }

    bool OrderManager::cancelOrder(const std::string& orderId) {
        auto it = orders_.find(orderId);

        if (it == orders_.end()) {
            return false;
        }

        if (it->second.status == OrderStatus::FILLED || it->second.status == OrderStatus::CANCELLED) {
            return false;
        }

        it->second.status = OrderStatus::CANCELLED;

        if (statusCallback_) {
            statusCallback_(it->second);
        }

        return true;
    }

    const Order* OrderManager::getOrder(const std::string& orderId) const {
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            return nullptr;
        }
        return &(it->second);
    }

    void OrderManager::setOrderStatusCallback(std::function<void(const Order&)> callback) {
        statusCallback_ = callback;
    }

    void OrderManager::setExecutionCallback(std::function<void(const Execution&)> callback) {
        executionCallback_ = callback;
    }

    void OrderManager::processExecution(const Execution& execution) {
        auto it = orders_.find(execution.orderId);
        if (it == orders_.end()) {
            return;
        }

        Order& order = it->second;

        order.filledQuantity += execution.quantity;

        if (order.filledQuantity >= order.quantity) {
            order.status = OrderStatus::FILLED;
        } else if (order.filledQuantity > 0) {
            order.status = OrderStatus::PARTIALLY_FILLED;
        }

        if (executionCallback_) {
            executionCallback_(execution);
        }

        if (statusCallback_) {
            statusCallback_(order);
        }
    }

    std::string OrderManager::generateOrderId() {
        int id = nextOrderId_++;
        return "ORDER-" + std::to_string(id);
    }
}