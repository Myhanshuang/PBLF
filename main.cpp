#include "mainwindow.h"
#include "playwindow.h"
#include "ResultPage.h"
#include <QApplication>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    PlayWindow w;
    w.show();
    w.startGame();
    // ResultPage w;
    // w.setResults(10, 10, 10, 10, 10, 100.00, 10, "S", 100, "Let the Wind Tell You", ":/Chart/testChart.png");
    // w.show();
    return a.exec();
}
