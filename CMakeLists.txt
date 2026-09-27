cmake_minimum_required(VERSION 3.28)
project(limit_order_book CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Drogon CONFIG REQUIRED)

add_library(core core/order_book.cpp core/matching_engine.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(limit_order_book main.cpp api/convert.cpp api/orders.cpp api/market_data.cpp)
target_link_libraries(limit_order_book PRIVATE core Drogon::Drogon)

add_executable(order_book_test test/order_book_test.cpp)
target_link_libraries(order_book_test PRIVATE core Drogon::Drogon)
