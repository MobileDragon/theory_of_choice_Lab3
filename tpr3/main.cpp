#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //w.setFixedSize(251,180);
    w.setFixedWidth(251);
    //w.setMinimumHeight(204);


    return a.exec();
}
