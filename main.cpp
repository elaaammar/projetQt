#include "mainwindow.h"
#include "connection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connection c;
    c.createconnect();
    MainWindow w;
    w.show();
    return a.exec();
}
