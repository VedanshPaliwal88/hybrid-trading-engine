#include "market_data/OrderBook.h"
#include "market_data/MarketDataSimulator.h"
#include "execution/OrderManager.h"
#include "execution/MarketSimulator.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace trading;

int main() {
    std::cout << "Market data test: " << std::endl;

    OrderBook orderBook("JPM");

    MarketDataSimulator dataSimulator("JPM", 261.04, 0.002);

    dataSimulator.setUpdateCallback([&orderBook](const MarketUpdate& update) {
        orderBook.update(update);
        // orderBook.print();
        // std::cout << "Best bid: " << orderBook.getBestBid()
        //             << " Best ask: " << orderBook.getBestAsk()
        //             << " Spread: " << orderBook.getSpread() << std::endl;
    });

    OrderManager orderManager;

    MarketSimulator marketSimulator;
    marketSimulator.registerOrderBook(&orderBook);

    marketSimulator.setExecutionCallback([&orderManager](const Execution& exec) {
        std::cout << "Execution: " << exec.executionId << ", Symbol: " << exec.symbol
                    << ", Price: " << exec.price << ", Quantity: " << exec.quantity << std::endl;
        orderManager.processExecution(exec);
    });

    orderManager.setOrderStatusCallback([](const Order& order) {
        std::cout << "Order: " << order.orderId << ", Status: ";
        switch(order.status) {
            case OrderStatus::CANCELLED: std::cout << "CANCELLED"; break;
            case OrderStatus::NEW: std::cout << "NEW"; break;
            case OrderStatus::FILLED: std::cout << "FILLED"; break;
            case OrderStatus::PARTIALLY_FILLED: std::cout << "PARTIALLY FILLED"; break;
            case OrderStatus::REJECTED: std::cout <<  "REJECTED"; break;
        }
        std::cout << ", Filled: " << order.filledQuantity << " / " << order.quantity << std::endl;
    });

    dataSimulator.start(100);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "\nCurrent Order Book: " << std::endl;
    orderBook.print();
    //testing a buy order
    std::string buyOrderId = orderManager.submitOrder("JPM", true, 0.0, 100, OrderType::MARKET);

    const Order* buyOrder = orderManager.getOrder(buyOrderId);
    if (buyOrder) {
        marketSimulator.submitOrder(*buyOrder);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //testing a sell order
    std::string sellOrderId = orderManager.submitOrder("JPM", false, orderBook.getBestBid() * 0.99, 5, OrderType::LIMIT);
    
    const Order* sellOrder = orderManager.getOrder(sellOrderId);
    if (sellOrder) {
        marketSimulator.submitOrder(*sellOrder);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    bool cancelResult = orderManager.cancelOrder(sellOrderId);
    std::cout << "cancel result: " << (cancelResult ? "success" : "failure") << std::endl;

    std::cout << "\nFinal order book: " << std::endl;
    orderBook.print();

    dataSimulator.stop();
    std::cout << "Done.";
    return 0;
}