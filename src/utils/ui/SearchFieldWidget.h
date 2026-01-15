#ifndef INVENTORY_SYSTEM_SEARCHFIELDWIDGET_H
#define INVENTORY_SYSTEM_SEARCHFIELDWIDGET_H
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include "../CommonEnums.h"

class SearchFieldWidget : public QWidget {
    Q_OBJECT

public:
    SearchFieldWidget(int index, bool searchBar = false, QWidget* parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        label_ = new QLabel("Parameter " + QString::number(index) + ":", this);
        typeComboBox_ = new QComboBox();
        modeComboBox_ = new QComboBox();
        caseSensitiveBox_ = new QCheckBox("Case Sensitivity");

        layout->addWidget(label_);
        layout->addWidget(modeComboBox_);

        if (searchBar) {
            lineEdit_ = new QLineEdit();
            layout->addWidget(lineEdit_);
        }

        layout->addWidget(typeComboBox_);
        layout->addWidget(caseSensitiveBox_);
        layout->addStretch();

        fillComboBoxes();
    }

    SearchType getSearchType() const
    {
        return static_cast<SearchType>(typeComboBox_->currentIndex());
    }

    SearchMode getSearchMode() const
    {
        return static_cast<SearchMode>(modeComboBox_->currentIndex());
    }

    bool getCaseSensitivity() const
    {
        return caseSensitiveBox_->isChecked();
    }

    QString getSearchText() const
    {
        if (lineEdit_ == nullptr) { return ""; }
        return lineEdit_->text();
    }

    void applyInventorySizes() const {
        if (lineEdit_) { lineEdit_->setFixedWidth(200); }
        if (modeComboBox_) { modeComboBox_->setFixedWidth(150); }
        if (typeComboBox_) { typeComboBox_->setFixedWidth(100); }
        if (caseSensitiveBox_) { caseSensitiveBox_->setFixedWidth(100); }
    }

private:
    QLabel* label_ = nullptr;
    QComboBox* typeComboBox_ = nullptr;
    QComboBox* modeComboBox_ = nullptr;
    QCheckBox* caseSensitiveBox_ = nullptr;
    QLineEdit* lineEdit_ = nullptr;

    void fillComboBoxes()
    {
        for (auto it = SearchModeLabels.begin(); it != SearchModeLabels.end(); it++) {
            int key = static_cast<int>(it.key());
            QString text = it.value();
            modeComboBox_->addItem(text, key);
        }
        for (auto it = SearchTypeLabels.begin(); it != SearchTypeLabels.end(); it++) {
            int key = static_cast<int>(it.key());
            QString text = it.value();
            typeComboBox_->addItem(text,key);
        }
    }

};


#endif //INVENTORY_SYSTEM_SEARCHFIELDWIDGET_H