#ifndef INVENTORY_SYSTEM_BASICTABLEVIEW_H
#define INVENTORY_SYSTEM_BASICTABLEVIEW_H
#include <QTableView>

#include "tablemodels/BasicTableModel.h"

class CustomTableView : public QTableView {
    Q_OBJECT
public:
    CustomTableView(BasicTableModel& model, QWidget* parent) : QTableView(parent)
    {
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);

        QTableView::setModel(&model);
    }

    int selectedRowIndex() const
    {
        QItemSelectionModel* selection = this->selectionModel();
        QModelIndexList rows = selection->selectedRows();

        int row;
        if (rows.isEmpty()) {
            row = -1;
        } else {
            row = rows.first().row();
        }

        return row;
    }
private:
};

#endif //INVENTORY_SYSTEM_BASICTABLEVIEW_H
