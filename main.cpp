#include "mainwindow.h"
#include "PlayWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    PlayWindow playWindow;
    playWindow.start();
    return a.exec();
}
