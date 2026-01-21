#ifndef INVENTORY_SYSTEM_BASICTABLEMODEL_H
#define INVENTORY_SYSTEM_BASICTABLEMODEL_H
#include <QAbstractTableModel>

#include "../../../database/DataBase.h"

class BasicTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BasicTableModel(DataBase& db, QObject* parent = nullptr)
    : QAbstractTableModel(parent), db_(db) {}

    virtual void loadData(const Search& search = {}) = 0;

    // removes singular row
    virtual bool removeRows(int row, int count, const QModelIndex& parent) = 0;

    // function returns -1 if row is out of index
    virtual int getId(int row) = 0;

protected:
    DataBase& db_;
};


#endif //INVENTORY_SYSTEM_BASICTABLEMODEL_H