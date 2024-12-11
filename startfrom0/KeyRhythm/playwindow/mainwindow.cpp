#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , playWindow(new PlayWindow(this))
    , resultsPage(new ResultsPage(this))
{
    // ui->setupUi(this);
    setupUi();

    connect(playWindow, & PlayWindow::gameOver, this, [this](){
        stackedWidget -> setCurrentWidget(resultsPage);
    });
}

void MainWindow::setupUi()
{
    stackedWidget = new QStackedWidget(this);
    playWindow = new PlayWindow(this);
    resultsPage = new ResultsPage(this);

    stackedWidget->addWidget(playWindow);
    stackedWidget->addWidget(resultsPage);

    setCentralWidget(stackedWidget);

    connect(playWindow, &PlayWindow::gameOver, [=]() {
        stackedWidget->setCurrentWidget(resultsPage);
        resultsPage->displayResults();
    });
}

MainWindow::~MainWindow()
{
    delete resultsPage;
    delete playWindow;
}
