cmake_minimum_required(VERSION 4.0)
project(inventory-system)

set(CMAKE_CXX_STANDARD 20)

add_executable(inventory-system src/main.cpp
        src/Inventory.cpp
        src/Inventory.h
        third-party/sqlite3.c
        src/DataBase.cpp
        src/DataBase.h
        src/Item.h)

include_directories(third-party)
