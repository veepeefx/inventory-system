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
        src/utils/Item.h
        src/ui/MainWindow.cpp
        src/ui/MainWindow.h
        src/ui/inventory/InventoryView.cpp
        src/ui/inventory/InventoryView.h
        src/ui/inventory/tablemodels/ItemTableModel.cpp
        src/ui/inventory/tablemodels/ItemTableModel.h
        src/ui/inventory/editors/ItemEditor.cpp
        src/ui/inventory/editors/ItemEditor.h
        src/database/SqlQueries.h
        src/database/SqlQueries.cpp
        src/utils/CommonEnums.h
        src/ui/settings/SettingsView.cpp
        src/ui/settings/SettingsView.h
        src/ui/inventory/tablemodels/ItemTypeTableModel.cpp
        src/ui/inventory/tablemodels/ItemTypeTableModel.h
        src/utils/ItemType.h
        src/ui/inventory/tablemodels/BasicTableModel.h
        src/ui/inventory/editors/ItemTypeEditor.cpp
        src/ui/inventory/editors/ItemTypeEditor.h
        src/ui/inventory/editors/BasicEditor.cpp
        src/ui/inventory/editors/BasicEditor.h
        src/utils/ui/UiTools.cpp
        src/utils/ui/UiTools.h
        src/utils/ui/SearchFieldWidget.h
        src/SettingsManager.cpp
        src/SettingsManager.h
        src/utils/Settings.h
        src/ui/inventory/CustomTableView.h)

include_directories(third-party)
target_link_libraries(inventory-system PRIVATE Qt6::Widgets)

