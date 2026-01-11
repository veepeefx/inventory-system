# Inventory system (C++ / Qt / SQLite3)

## Description
Inventory Management System implemented in C++ using SQLite3 as the database backend. The system 
allows users to efficiently keep track of items stored in an inventory, including adding, updating,
viewing, and removing items. The application is designed as a lightweight, local solution that does
not require a separate database server, making it suitable for small businesses, personal projects,
or educational purposes.

## Features
- Item database for small inventory
- Simple and clean Qt-based user interface

#### Future additions
- Item types to group items
- Settings to modify presets

## Disclaimer
This software is provided as open source and “as is”, without any guarantees regarding data safety,
compatibility, or future support. The developer is not responsible for any data loss, corruption,
or unintended changes to data stored using this system.

## Installation / Build
1. Clone this repo
   ```
   git clone https://github.com/veepeefx/inventory-system
   cd inventory-system
   ```

2. Create a build directory and run CMake
    ```
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

3. Run the executable
    ```
    ./inventory-system
    ```

## Dependencies
All libraries are included in the project, so **no external installation is required**:
- [SQLite3](https://sqlite.org/download.html) - SQL library (included as `sqlite3.c` and `sqlite3.h`)
- C++ 20 compatible compiler
- CMake 4.0 ->
- Qt6 with the following required modules:
    - Core
    - Gui
    - Widgets

## License
Licensed under MIT License.
