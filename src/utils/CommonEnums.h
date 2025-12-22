#ifndef INVENTORY_SYSTEM_COMMONENUMS_H
#define INVENTORY_SYSTEM_COMMONENUMS_H

#include <QMap>

enum class SearchMode {
    NAME,
    PRODUCT_NUMBER,
    SELF_LOCATION,
    EAN,
    ID
};

static const QMap<SearchMode, QString> SearchModeLabels {
            { SearchMode::NAME, "Name" },
            { SearchMode::PRODUCT_NUMBER, "Product Number" },
            { SearchMode::SELF_LOCATION, "Self Location"},
            { SearchMode::EAN, "EAN"},
            { SearchMode::ID, "Unique ID"}
};

enum class SearchType {
    STARTS_WITH,
    CONTAINS
};


#endif //INVENTORY_SYSTEM_COMMONENUMS_H