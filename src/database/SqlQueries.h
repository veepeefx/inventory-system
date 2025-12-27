#ifndef INVENTORY_SYSTEM_SQLQUERIES_H
#define INVENTORY_SYSTEM_SQLQUERIES_H

#include <string>
#include <vector>

#include "../utils/Item.h"
#include "../utils/CommonEnums.h"


namespace Sql::Items {

    constexpr const char* CREATE_TABLE =  R"(
    CREATE TABLE IF NOT EXISTS items (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        product_number TEXT,
        quantity INTEGER DEFAULT 0,
        ean TEXT,
        self_location TEXT,
        price_no_vat REAL DEFAULT 0.0,
        vat REAL DEFAULT 0.0,
        discount REAL DEFAULT 0.0,
        description TEXT,
        modified_at TEXT DEFAULT (datetime('now', 'localtime')),
        created_at TEXT DEFAULT (datetime('now', 'localtime')));
    )";

    constexpr const char* INSERT = R"(
    INSERT INTO items (
        name, product_number, quantity, ean, self_location,
        price_no_vat, vat, discount, description)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    constexpr const char* REMOVE = "DELETE FROM items WHERE id = ?;";

    constexpr const char* GET = R"(
    SELECT id, name, product_number, quantity, ean, self_location,
        price_no_vat, vat, discount, description, modified_at, created_at
    FROM items;
    )";

    std::string buildDynamicUpdateSql(const ItemUpdate& item);
    std::string buildDynamicSearchSql(const std::vector<std::string>& searches,
                                      const std::vector<SearchMode>& modes,
                                      const std::vector<SearchType>& types,
                                      const std::vector<bool> &vCaseSensitivity,
                                      std::vector<std::string>& outBindValues);

}

#endif //INVENTORY_SYSTEM_SQLQUERIES_H