#ifndef INVENTORY_SYSTEM_POPUPDIALOG_H
#define INVENTORY_SYSTEM_POPUPDIALOG_H
#include <QMessageBox>

enum class PopUpCode {
    SETTINGS_RESTART,
    MISSING_INFO_ITEM,
    MISSING_INFO_ITEM_TYPE,
    ITEM_HAS_ITEM_TYPE,
    CONFIRM_TO_REMOVE_ITEM,
    CONFIRM_TO_REMOVE_ITEM_TYPE
};

class PopUpMessage {
public:
    static void info(PopUpCode code, QWidget* parent = nullptr) {
        QMessageBox msg(parent);
        msg.setWindowTitle("Information");
        msg.setText(popUpCodeToKey(code));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

    static bool confirm(PopUpCode code, QWidget* parent = nullptr) {
        QMessageBox msg(parent);
        msg.setWindowTitle("Confirm");
        msg.setText(popUpCodeToKey(code));
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Ok);

        msg.exec();
        return msg.clickedButton()
            && msg.standardButton(msg.clickedButton()) == QMessageBox::Ok;
    }

private:

    static QString popUpCodeToKey(PopUpCode code)
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

            case PopUpCode::ITEM_HAS_ITEM_TYPE:
                return "This item already has item type!\n"
                       "Are you sure you want to add it?";

            case PopUpCode::CONFIRM_TO_REMOVE_ITEM:
                return"You are removing item permanently!\n"
                       "Are you sure you want to remove this item?";

            case PopUpCode::CONFIRM_TO_REMOVE_ITEM_TYPE:
                return "You are removing item type permanently!\n"
                       "Are you sure you want to remove this item type?";

            default: return "Missing pop up message";
        }
    }
};

#endif //INVENTORY_SYSTEM_POPUPDIALOG_H