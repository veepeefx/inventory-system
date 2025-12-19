#include "SqlQueries.h"
#include "../Item.h"

#include <string>

namespace {

    constexpr const char* ITEMS_UPDATE_FIRST = "UPDATE items SET ";
    constexpr const char* ITEMS_UPDATE_LAST = ", modified_at = datetime('now', 'localtime')"
                                              " WHERE id = ?";
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
    }


}
