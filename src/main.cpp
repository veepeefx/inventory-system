#include <QApplication>

#include "SettingsManager.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SettingsManager::loadSettings();

    DataBase db("database.db");
    MainWindow window(db);

    window.show();
    return QApplication::exec();
}