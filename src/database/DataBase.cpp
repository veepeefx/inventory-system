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

bool DataBase::insert(const ItemType& itemType)
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
    return true;
}

bool DataBase::remove(const Item& item)
{
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, Sql::Items::REMOVE, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // checks that id is valid
    if (sqlite3_bind_int(stmt, 1, item.id) != SQLITE_OK) {
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

bool DataBase::remove(const ItemType& itemType)
{
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, Sql::ItemTypes::REMOVE, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, itemType.id) != SQLITE_OK) {
        std::cerr << "Failed to bind id: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int deletedRows = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    if (deletedRows == 0) {
        return false;
    }

    return true;
}

bool DataBase::updateItem(const ItemUpdate& updateItem, const int& itemId,
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
        return false;
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

std::vector<ItemType> DataBase::getItemTypes()
{
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, Sql::Items::GET, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return {};
    }

    std::vector<ItemType> itemTypes = fillItemTypesToVector(stmt);

    sqlite3_finalize(stmt);
    return itemTypes;
}

std::vector<Item> DataBase::searchItems(const std::vector<std::string>& searches,
                                        const std::vector<SearchMode>& modes,
                                        const std::vector<SearchType>& types,
                                        const std::vector<bool>& vCaseSensitivity)
{
    std::vector<std::string> bindValues;
    sqlite3_stmt* stmt;

    std::string sql = Sql::Items::buildDynamicSearchSql(searches, modes, types,
                                                        vCaseSensitivity, bindValues);

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
        sqlite3_bind_int(stmt, index++, item.itemTypeId.value());
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