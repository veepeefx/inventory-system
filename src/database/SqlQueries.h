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
        item_type_id INTEGER,
        name TEXT,
        product_number TEXT,
        quantity INTEGER DEFAULT 0,
        ean TEXT,
        self_location TEXT,
        price_no_vat REAL DEFAULT 0.0,
        vat REAL DEFAULT 0.0,
        discount REAL DEFAULT 0.0,
        description TEXT,
        modified_at TEXT DEFAULT (datetime('now', 'localtime')),
        created_at TEXT DEFAULT (datetime('now', 'localtime')),
        FOREIGN KEY (item_type_id)
            REFERENCES item_types(id)
            ON DELETE SET NULL);
    )";

    constexpr const char* CREATE_INDEX_ITEM_TYPE_ID = R"("
    CREATE INDEX IF NOT EXISTS idx_items_item_type_id
        ON items(item_type_id);
    )";

    constexpr const char* INSERT = R"(
    INSERT INTO items (
        name, item_type_id, product_number, quantity, ean, self_location,
        price_no_vat, vat, discount, description)
    VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    constexpr const char* REMOVE = "DELETE FROM items WHERE id = ?;";

    constexpr const char* GET = R"(
    SELECT id, item_type_id, name, product_number, quantity, ean, self_location,
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

namespace Sql::ItemTypes {

    constexpr const char* CREATE_TABLE = R"(
    CREATE TABLE IF NOT EXISTS item_types (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT,
        type_number TEXT,
        description TEXT,
        modified_at TEXT DEFAULT (datetime('now', 'localtime')),
        created_at TEXT DEFAULT (datetime('now', 'localtime')));
    )";

    constexpr const char* INSERT = R"(
    INSERT INTO item_types (
        id, name, type_number, description, modified_at, created_at)
    VALUES (?, ?, ?, ?, ?, ?);
    )";

    constexpr const char* REMOVE = "DELETE FROM item_types WHERE id = ?;";

    constexpr const char* GET = R"(
    SELECT t.id, t.name, t.type_number, t.description, t.modified_at, t.created_at,
        COALESCE(SUM(i.quantity), 0) AS total_quantity
    FROM item_types t
    LEFT JOIN items i
        ON i.item_type_id = t.id
    GROUP BY t.id;
    )";

}

#endif //INVENTORY_SYSTEM_SQLQUERIES_H