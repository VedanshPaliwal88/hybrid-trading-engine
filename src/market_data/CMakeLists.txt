add_library(market_data
    OrderBook.cpp
    MarketDataSimulator.cpp
)

target_include_directories(market_data PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
