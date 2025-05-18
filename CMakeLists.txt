cmake_minimum_required(VERSION 3.10)
project(HybridTradingSystem VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)

# Optional: Enable testing
enable_testing()
add_subdirectory(test)

