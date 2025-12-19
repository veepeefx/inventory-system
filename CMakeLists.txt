cmake_minimum_required(VERSION 4.0)
project(inventory-system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

add_executable(inventory-system src/main.cpp
        third-party/sqlite3.c
        src/database/DataBase.cpp
        src/database/DataBase.h
        src/Item.h
        src/View/MainWindow.cpp
        src/View/MainWindow.h
        src/View/InventoryView.cpp
        src/View/InventoryView.h
        src/View/ItemTableModel.cpp
        src/View/ItemTableModel.h
        src/View/ItemEditorDialog.cpp
        src/View/ItemEditorDialog.h
        src/database/SqlQueries.h
        src/database/SqlQueries.cpp)

include_directories(third-party)
target_link_libraries(inventory-system PRIVATE Qt6::Widgets)

