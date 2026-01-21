#include "DataBase.h"

#include "sqlite3.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "../utils/Item.h"
#include "SqlQueries.h"

DataBase::DataBase(const std::string& filePath)
{
    // trying to open database
    int open = sqlite3_open(filePath.c_str(), &db);
    if (open != SQLITE_OK) {
        std::cerr << "Can't open database: " << filePath
                  << " Error: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::cerr << "Opened database successfully." << std::endl;
    initTables();
}

DataBase::~DataBase()
{
    sqlite3_close(db);
}

void DataBase::initTables()
{
    // setting foreign keys on
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

    // item types
    sqlite3_exec(db, Sql::ItemTypes::CREATE_TABLE, nullptr, nullptr, nullptr);

    // items
    sqlite3_exec(db, Sql::Items::CREATE_TABLE, nullptr, nullptr, nullptr);
    sqlite3_exec(db, Sql::Items::CREATE_INDEX_ITEM_TYPE_ID, nullptr, nullptr, nullptr);
}

bool DataBase::insert(const Item& item)
{
    // must have name or product number in order to add
    if (item.name.empty() && item.productNumber.empty()) {
        return false;
    }

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, Sql::Items::INSERT, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int index = 1;
    sqlite3_bind_text(stmt, index++, item.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, index++, item.itemTypeId);
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

bool DataBase::insert(ItemType& itemType)
{
    // must have name or type number in order to add
    if (itemType.name.empty() && itemType.typeNumber.empty()) {
        return false;
    }

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, Sql::ItemTypes::INSERT, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int index = 1;
    sqlite3_bind_text(stmt, index++, itemType.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, itemType.typeNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, itemType.selfLocation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, itemType.description.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    // adding autoincremented id for itemtype
    itemType.id = static_cast<int>(sqlite3_last_insert_rowid(db));

    return true;
}

bool DataBase::remove(int id, InventoryMode mode)
{
    const char* sql;
    switch (mode) {
        case InventoryMode::ITEM:       sql = Sql::Items::REMOVE;       break;
        case InventoryMode::ITEM_TYPE:  sql = Sql::ItemTypes::REMOVE;   break;
        default: return false;
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // checks that id is valid
    if (sqlite3_bind_int(stmt, 1, id) != SQLITE_OK) {
        std::cerr << "Failed to bind id: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // checks that deletion is successful
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


bool DataBase::update(const ItemUpdate& updateItem, int itemId,
                      const std::string& orgName, const std::string& orgProdNumber)
{
    // if after updating both name and product number are empty dont make the update
    std::string newName = updateItem.name ? *updateItem.name : orgName;
    std::string newProductNumber = updateItem.productNumber ? *updateItem.productNumber : orgProdNumber;

    if (newName.empty() && newProductNumber.empty()) {
        return false;
    }

    std::string updateSql = Sql::Items::buildDynamicUpdateSql(updateItem);

    // if nothing to update ending update
    if (updateSql.empty()) {
        std::cerr << "Nothing to update" << std::endl;
        return true;
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        return false;
    }

    int indexForId = dynamicUpdateBinding(updateItem, stmt);
    sqlite3_bind_int(stmt, indexForId, itemId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


bool DataBase::update(const ItemTypeUpdate& updateType, int itemId,
                      const std::string& orgName, const std::string& orgTypeNumber)
{
    std::string newName = updateType.name ? *updateType.name : orgName;
    std::string newProductNumber = updateType.typeNumber ? *updateType.typeNumber : orgTypeNumber;

    if (newName.empty() && newProductNumber.empty()) {
        return false;
    }

    std::string updateSql = Sql::ItemTypes::buildDynamicUpdateSql(updateType);

    // if nothing to update ending update
    if (updateSql.empty()) {
        std::cerr << "Nothing to update" << std::endl;
        return true;
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement." << std::endl;
        return false;
    }

    int indexForId = dynamicUpdateBinding(updateType, stmt);
    sqlite3_bind_int(stmt, indexForId, itemId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DataBase::updateItemTypes(int itemTypeId, const std::vector<int> &itemIds)
{
    bool allSucceeds = true;
    for (int itemId : itemIds) {
        ItemUpdate updatedItem;
        updatedItem.itemTypeId = itemTypeId;

        Item item = getItem(itemId);
        allSucceeds = update(updatedItem, itemId, item.name, item.productNumber);
    }

    return allSucceeds;
}

std::vector<Item> DataBase::getItems()
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, Sql::Items::GET, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    std::vector<Item> items = fillItemsToVector(stmt);

    sqlite3_finalize(stmt);
    return items;
}

Item DataBase::getItem(int id)
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, Sql::Items::GET_ITEM, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    sqlite3_bind_int(stmt, 1, id);

    std::vector<Item> item = fillItemsToVector(stmt);

    sqlite3_finalize(stmt);
    return item.front();    // returning the only element
}

std::vector<ItemType> DataBase::getItemTypes()
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, Sql::ItemTypes::GET, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    std::vector<ItemType> itemTypes = fillItemTypesToVector(stmt);

    sqlite3_finalize(stmt);
    return itemTypes;
}

std::vector<Item> DataBase::searchItems(const Search& search)
{
    std::vector<std::string> bindValues;
    sqlite3_stmt* stmt;

    std::string sql = Sql::Items::buildDynamicSearchSql(search, bindValues);

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    int index = 1;
    for (const std::string& bind : bindValues) {
        sqlite3_bind_text(stmt, index++, bind.c_str(), -1, SQLITE_TRANSIENT);
    }

    std::vector<Item> items = fillItemsToVector(stmt);

    sqlite3_finalize(stmt);
    return items;
}


int DataBase::dynamicUpdateBinding(const ItemUpdate &item, sqlite3_stmt* stmt)
{
    int index = 1;

    if (item.name) {
        sqlite3_bind_text(stmt, index++, item.name->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (item.itemTypeId) {
        if (item.itemTypeId == -1) {
            sqlite3_bind_null(stmt, index++);
        } else {
            sqlite3_bind_int(stmt, index++, item.itemTypeId.value());
        }
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

int DataBase::dynamicUpdateBinding(const ItemTypeUpdate& type, sqlite3_stmt* stmt)
{
    int index = 1;

    if (type.name) {
        sqlite3_bind_text(stmt, index++, type.name->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (type.typeNumber) {
        sqlite3_bind_text(stmt, index++, type.typeNumber->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (type.selfLocation) {
        sqlite3_bind_text(stmt, index++, type.selfLocation->c_str(), -1, SQLITE_TRANSIENT);
    }
    if (type.description) {
        sqlite3_bind_text(stmt, index++, type.description->c_str(), -1, SQLITE_TRANSIENT);
    }

    return index;
}

std::vector<Item> DataBase::fillItemsToVector(sqlite3_stmt* stmt)
{
    std::vector<Item> items;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Item item;

        int index = 0;
        item.id = sqlite3_column_int(stmt, index++);
        item.itemTypeId = sqlite3_column_int(stmt, index++);
        item.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.productNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.quantity = sqlite3_column_int(stmt, index++);
        item.ean = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.selfLocation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.priceNoVat = sqlite3_column_double(stmt, index++);
        item.vat = sqlite3_column_double(stmt, index++);
        item.discount = sqlite3_column_double(stmt, index++);
        item.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.modifiedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        item.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));

        items.push_back(item);
    }

    return items;
}

std::vector<ItemType> DataBase::fillItemTypesToVector(sqlite3_stmt* stmt)
{
    std::vector<ItemType> itemTypes;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ItemType type;

        int index = 0;
        type.id = sqlite3_column_int(stmt, index++);
        type.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.typeNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.selfLocation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.modifiedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        type.totalQuantity = sqlite3_column_int(stmt, index++);

        // split ids and push to items vector
        const char* itemIdsCSV = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
        if(itemIdsCSV) {
            std::stringstream ss(itemIdsCSV);
            std::string token;
            while(std::getline(ss, token, ',')) {
                type.items.push_back(std::stoi(token));
            }
        }
        itemTypes.push_back(type);
    }
    return itemTypes;
}