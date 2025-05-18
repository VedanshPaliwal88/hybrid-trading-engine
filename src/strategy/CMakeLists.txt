add_library(strategy
    StrategyManager.cpp
    MarketMakingStrategy.cpp
)

target_include_directories(strategy PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
