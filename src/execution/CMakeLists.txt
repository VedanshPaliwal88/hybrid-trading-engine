add_library(execution
    OrderManager.cpp
    MarketSimulator.cpp
)

target_include_directories(execution PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(execution
    market_data
)