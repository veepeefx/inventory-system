#include "SqlQueries.h"

#include <iostream>

#include "../utils/Item.h"

#include <string>

namespace {

    constexpr const char* ITEMS_UPDATE_FIRST = "UPDATE items SET ";
    constexpr const char* ITEMS_UPDATE_LAST = ", modified_at = datetime('now', 'localtime')"
                                              " WHERE id = ?";

    constexpr const char* ITEMS_SEARCH = R"(
    SELECT id, name, product_number, quantity, ean, self_location,
        price_no_vat, vat, discount, description, modified_at, created_at
    FROM items WHERE )";

}

namespace Sql {
    namespace Items {

        std::string buildDynamicUpdateSql(const ItemUpdate& item)
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

            if (res.empty()) {
                return "";
            }
            return ITEMS_UPDATE_FIRST + res + ITEMS_UPDATE_LAST;
        }

        std::string buildDynamicSearchSql(const std::vector<std::string> &searches,
                                          const std::vector<SearchMode> &modes,
                                          const std::vector<SearchType> &types,
                                          const std::vector<bool> &vCaseSensitivity,
                                          std::vector<std::string>& outBindValues)
        {
            std::string sql = "";

            for (int i = 0; i < searches.size(); i++) {
                if (i != 0) {
                    sql += " AND ";
                }

                switch (modes[i]) {
                    case SearchMode::NAME:              sql += "name ";             break;
                    case SearchMode::PRODUCT_NUMBER:    sql += "product_number ";   break;
                    case SearchMode::SELF_LOCATION:     sql += "self_location ";    break;
                    case SearchMode::EAN:               sql += "ean ";              break;
                    case SearchMode::ID:                sql += "id ";               break;
                };

                bool cs = vCaseSensitivity[i];
                std::string pattern;

                switch (types[i]) {
                    case SearchType::STARTS_WITH:
                        sql += cs ? "GLOB ?" : "LIKE ?";
                        pattern = searches[i] + (cs ? "*" : "%");
                        break;
                    case SearchType::ENDS_WITH:
                        sql += cs ? "GLOB ?" : "LIKE ?";
                        pattern = (cs ? "*" : "%") + searches[i];
                        break;
                    case SearchType::CONTAINS:
                        sql += cs ? "GLOB ?" : "LIKE ?";
                        pattern = (cs ? "*" : "%") + searches[i] + (cs ? "*" : "%");
                        break;
                    case SearchType::EQUALS:
                        sql += "= ?";
                        sql += cs ? " COLLATE BINARY" : " COLLATE NOCASE";
                        pattern = searches[i];
                        break;
                };
                outBindValues.push_back(pattern);
            }
            sql += ";";
            std::cerr << ITEMS_SEARCH + sql << std::endl;
            return ITEMS_SEARCH + sql;
        }
    }
}
