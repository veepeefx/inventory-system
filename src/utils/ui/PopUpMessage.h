#ifndef INVENTORY_SYSTEM_POPUPDIALOG_H
#define INVENTORY_SYSTEM_POPUPDIALOG_H
#include <QMessageBox>

enum class PopUpCode {
    SETTINGS_RESTART,
    MISSING_INFO_ITEM,
    MISSING_INFO_ITEM_TYPE
};

class PopUpMessage : public QMessageBox {
    Q_OBJECT
public:
    PopUpMessage(PopUpCode code, QWidget* parent = nullptr) : QMessageBox(parent)
    {
        QString msg = popUpCodeToKey(code);
        QMessageBox::information(parent, "Information", msg, QMessageBox::Ok);
    }

private:

    QString popUpCodeToKey(PopUpCode code)
    {
        switch (code) {
            case PopUpCode::SETTINGS_RESTART:
                return "The settings have been updated.\n"
                       "Please restart the application for the new settings to take effect.";

            case PopUpCode::MISSING_INFO_ITEM:
                return "Cannot save item:\n"
                       "At least a name or a product number is required.";

            case PopUpCode::MISSING_INFO_ITEM_TYPE:
                return "Cannot save item type:\n"
                       "At least a name or a type number is required.";

            default: return "Missing pop up message";
        }
    }
};

#endif //INVENTORY_SYSTEM_POPUPDIALOG_H