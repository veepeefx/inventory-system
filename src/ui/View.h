#ifndef INVENTORY_SYSTEM_VIEW_H
#define INVENTORY_SYSTEM_VIEW_H

class View : public QWidget {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr) : QWidget(parent) {}

    ~View() override = default;

    // function used to clear views when shown
    virtual void viewShown() = 0;

signals:
    // signal to return main menu
    void returnMainMenu();

};

#endif //INVENTORY_SYSTEM_VIEW_H