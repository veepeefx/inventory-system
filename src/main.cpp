#include <QApplication>

#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DataBase db("database.db");
    MainWindow window(&db);

    window.show();
    return QApplication::exec();
}