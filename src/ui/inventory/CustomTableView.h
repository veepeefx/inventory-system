#ifndef INVENTORY_SYSTEM_BASICTABLEVIEW_H
#define INVENTORY_SYSTEM_BASICTABLEVIEW_H
#include <QTableView>

#include "tablemodels/BasicTableModel.h"

class CustomTableView : public QTableView {
    Q_OBJECT
public:
    CustomTableView(QWidget* parent) : QTableView(parent)
    {
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    void setModel(BasicTableModel& model)
    {
        QTableView::setModel(&model);
    }

    int selectedRowIndex() const
    {
        if (QItemSelectionModel* selection = this->selectionModel()) {
            QModelIndexList rows = selection->selectedRows();
            if (!rows.isEmpty()) {
                return rows.first().row();
            }
        }
        return -1;
    }
};

#endif //INVENTORY_SYSTEM_BASICTABLEVIEW_H
