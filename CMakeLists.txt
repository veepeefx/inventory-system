cmake_minimum_required(VERSION 4.0)
project(inventory-system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

add_executable(inventory-system src/main.cpp
        src/Inventory.cpp
        src/Inventory.h
        third-party/sqlite3.c
        src/DataBase.cpp
        src/DataBase.h
        src/Item.h
        src/MainWindow.cpp
        src/MainWindow.h)

include_directories(third-party)
target_link_libraries(inventory-system PRIVATE Qt6::Widgets)

