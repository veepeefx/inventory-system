#include "DataBase.h"
#include "sqlite3.h"

#include <iostream>
#include <ostream>
#include <string>

#include "Item.h"

DataBase::DataBase(const std::string& filePath)
{
    // trying to open database
    int open = sqlite3_open(filePath.c_str(), &db);
    if (open != SQLITE_OK) {
        std::cerr << "Can't open database: " << filePath
                  << " Error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::cout << "Opened database successfully." << std::endl;
    initTables();
}

DataBase::~DataBase()
{
    sqlite3_close(db);
}

void DataBase::initTables()
{
    const char* sqlCreateItems = R"(
    CREATE TABLE IF NOT EXISTS items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        product_number TEXT,
        quantity INTEGER NOT NULL DEFAULT 0,
        ean TEXT,
        self_location TEXT,
        price_no_vat REAL DEFAULT 0.0,
        vat REAL DEFAULT 0.0,
        discount REAL NOT NULL DEFAULT 0.0,
        modified_at TEXT DEFAULT (datetime('now', 'localtime')),
        created_at TEXT DEFAULT (datetime('now', 'localtime'))); )";

    sqlite3_exec(db, sqlCreateItems, NULL, NULL, NULL);
}

bool DataBase::insertItem(const Item& item)
{
    const char* sqlInsert = R"(
    INSERT INTO items (name, product_number, quantity, ean, self_location, price_no_vat, vat, discount)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?); )";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
    }

    sqlite3_bind_text(stmt, 1, item.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.productNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, item.quantity);
    sqlite3_bind_text(stmt, 4, item.ean.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, item.selfLocation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, item.priceNoVat);
    sqlite3_bind_double(stmt, 7, item.vat);
    sqlite3_bind_double(stmt, 8, item.discount);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::deleteItem(const int& itemId)
{
    const char* sqlRemove = "DELETE FROM items WHERE id = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sqlRemove, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // checks that id is valid
    if (sqlite3_bind_int(stmt, 1, itemId) != SQLITE_OK) {
        std::cerr << "Failed to bind id: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // checks that deletation is successful
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::list_all()
{
    const char* sqlListAll = "SELECT * FROM items";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sqlListAll, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "ID: " << sqlite3_column_int(stmt, 0)
        << ", Name: " << sqlite3_column_text(stmt, 1)
        << ", Product number: " << sqlite3_column_text(stmt, 2)
        << ", Quantity: " << sqlite3_column_int(stmt, 3)
        << ", Ean: " << sqlite3_column_text(stmt, 4)
        << ", Self location: " << sqlite3_column_text(stmt, 5)
        << ", Price no vat: " << sqlite3_column_double(stmt, 6)
        << ", Vat: " << sqlite3_column_double(stmt, 7)
        << ", Discount: " << sqlite3_column_double(stmt, 8)
        << ", Modified at: " << sqlite3_column_text(stmt, 9)
        << ", Created at: " << sqlite3_column_text(stmt, 10)
        << std::endl;
    }

    sqlite3_finalize(stmt);
    return true;
}
