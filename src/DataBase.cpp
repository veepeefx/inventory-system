#include "DataBase.h"

#include <functional>

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
        description TEXT,
        modified_at TEXT DEFAULT (datetime('now', 'localtime')),
        created_at TEXT DEFAULT (datetime('now', 'localtime'))); )";

    sqlite3_exec(db, sqlCreateItems, NULL, NULL, NULL);
}

bool DataBase::insertItem(const Item& item)
{
    const char* sqlInsert = R"(
    INSERT INTO items (name, product_number, quantity, ean, self_location, price_no_vat,
                       vat, discount, description) VALUES (?, ?, ?, ?, ?, ?, ?, ?); )";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
    }

    int index = 1;
    sqlite3_bind_text(stmt, index++, item.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, item.productNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, index++, item.quantity);
    sqlite3_bind_text(stmt, index++, item.ean.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, item.selfLocation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, index++, item.priceNoVat);
    sqlite3_bind_double(stmt, index++, item.vat);
    sqlite3_bind_double(stmt, index++, item.discount);
    sqlite3_bind_text(stmt, index, item.description.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::updateItem(const int& id, const ItemUpdate& item)
{
    std::string sqlDynamicPart = dynamicUpdateSqlStr(item);

    // if nothing to update ending update
    if (sqlDynamicPart.empty()) {
        std::cerr << "Nothing to update" << std::endl;
        return false;
    }

    std::string sql = "UPDATE items SET " + sqlDynamicPart +
                      ", modified_at = datetime('now', 'localtime') WHERE id = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        return false;
    }

    int indexForId = dynamicUpdateBinding(item, stmt);
    sqlite3_bind_int(stmt, indexForId, id);

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

    int deletedRows = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    // checks that in the list had item with itemId
    if (deletedRows == 0) {
        return false;
    }

    return true;
}

std::vector<Item> DataBase::getItems()
{
    std::vector<Item> items;

    const char* sql = "SELECT id, name, product_number, quantity, self_location, "
                      "price_no_vat, vat, discount, description FROM items;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Item item;
        item.id = sqlite3_column_int(stmt, 0);
        item.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        item.productNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        item.quantity = sqlite3_column_int(stmt, 3);
        item.selfLocation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        item.priceNoVat = sqlite3_column_double(stmt, 5);
        item.vat = sqlite3_column_double(stmt, 6);
        item.discount = sqlite3_column_double(stmt, 7);
        item.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));

        items.push_back(item);
    }

    sqlite3_finalize(stmt);
    return items;
}

std::string DataBase::dynamicUpdateSqlStr(const ItemUpdate &item)
{
    std::string res;

    auto addField = [&](const std::string& fieldName) {
        if (!res.empty()) {
            res += ", ";
        }
        res += fieldName + " = ?";
    };

    if (item.name) { addField("name"); }
    if (item.productNumber) { addField("product_number"); }
    if (item.quantity) { addField("quantity"); }
    if (item.ean) { addField("ean"); }
    if (item.selfLocation) { addField("self_location"); }
    if (item.priceNoVat) { addField("price_no_vat"); }
    if (item.vat) { addField("vat"); }
    if (item.discount) { addField("discount"); }
    if (item.description) {addField("description"); }

    return res;
}

int DataBase::dynamicUpdateBinding(const ItemUpdate &item, sqlite3_stmt* stmt)
{
    int index = 1;

    if (item.name) {
        sqlite3_bind_text(stmt, index++, item.name->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (item.productNumber) {
        sqlite3_bind_text(stmt, index++, item.productNumber->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (item.quantity) {
        sqlite3_bind_int(stmt, index++, item.quantity.value());
    }
    if (item.ean) {
        sqlite3_bind_text(stmt, index++, item.ean->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (item.selfLocation) {
        sqlite3_bind_text(stmt, index++, item.selfLocation->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (item.priceNoVat) {
        sqlite3_bind_double(stmt, index++, item.priceNoVat.value());
    }
    if (item.vat) {
        sqlite3_bind_double(stmt, index++, item.vat.value());
    }
    if (item.discount) {
        sqlite3_bind_double(stmt, index++, item.discount.value());
    }
    if (item.description) {
        sqlite3_bind_text(stmt, index++, item.description->c_str(), -1, SQLITE_TRANSIENT);
    }

    return index;
}