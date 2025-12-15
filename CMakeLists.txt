cmake_minimum_required(VERSION 4.0)
project(inventory_system)

set(CMAKE_CXX_STANDARD 20)

add_executable(inventory_system src/main.cpp
        src/Inventory.cpp
        src/Inventory.h)
