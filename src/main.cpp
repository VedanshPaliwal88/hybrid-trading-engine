#include "market_data/OrderBook.h"
#include "market_data/MarketDataSimulator.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace trading;

int main() {
    std::cout << "Market data test: " << std::endl;

    OrderBook orderBook("JPM");

    MarketDataSimulator simulator("JPM", 261.04, 0.002);

    simulator.setUpdateCallback([&orderBook](const MarketUpdate& update) {
        orderBook.update(update);
        orderBook.print();
        std::cout << "Best bid: " << orderBook.getBestBid()
                    << " Best ask: " << orderBook.getBestAsk()
                    << " Spread: " << orderBook.getSpread() << std::endl;
    });

    simulator.start(100);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    simulator.stop();

    std::cout << "Done.";
    return 0;
}