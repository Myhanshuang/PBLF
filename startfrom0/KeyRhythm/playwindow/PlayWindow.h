#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include "Chart.h"

class PlayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayWindow(QWidget *parent = nullptr);
    ~PlayWindow();

    void loadChart(const Chart& chart);
    void startGame();
    void stopGame();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    Chart currentChart;
    bool gameRunning = false;
    int elapsedTime = 0;  // Time elapsed since the game started
    int currentMeasureIndex = 0;

    QTimer *gameTimer;  // Timer to update game logic and refresh UI
    QVector<Result> results;  // To store results of the notes (for scoring)

    void processGameLogic();
    void checkNoteHit();
    void drawNotes(QPainter& painter);
    void calculateScore();
};

#endif // PLAYWINDOW_H
