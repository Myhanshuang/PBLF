#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "PlayWindow.h"
#include "ResultsPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void gameOver();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStackedWidget *stackedWidget;  // For switching between pages
    PlayWindow *playWindow;         // The game screen
    ResultsPage *resultsPage;       // The results screen

private:
    void setupUi();
};
#endif // MAINWINDOW_H
