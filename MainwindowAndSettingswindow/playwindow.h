#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include "Chart.h"
#include "ClickablePixmapItem.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QFile>
#include<QFileOpenEvent>
#include <QDebug>
#include <Qfontdatabase>
#include <Qpixmap>
#include <QPushButton>
#include <qapplication.h>
#include <windows.h>
//#include <QtMultimedia/QtMultimedia>

// Replace QGraphicsPixmapItem* with ClickablePixmapItem*



class PlayWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit PlayWindow(QWidget* parent = nullptr);
    ~PlayWindow();

    void loadChart();
    void startGame();
    void addFileSource(QString path);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void setupUI();
    void updateGame();
    void spawnNotes();
    void checkJudgment();
    void updateStats();
    void showPauseMenu();
    void hidePauseMenu();
    void drawChannels();
    void checkCollisions();
    QPixmap setOpacityImage(const QString &imagePath, qreal opacity);
    QPixmap createDarkenedImage(const QString& imagePath, qreal opacity = 0.5);
    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsLineItem* checkerLine;
    QTimer* gameTimer;
    QLabel* statsLabel;

    QString fileSource;
    Chart currentChart;
    // QMediaPlayer* musicPlayer;
    QVector<QGraphicsRectItem*> notes;
    int gameTime;

    // Pause Menu
    QGraphicsRectItem* pauseMenuBackground;
    ClickablePixmapItem* continueButton;
    ClickablePixmapItem* restartButton;
    ClickablePixmapItem* exitButton;
    bool isPaused;
    QMap<int, int> keyToColumn;

    //in game
    int combo = 0;
    int score = 0;
    float accuracy = 0;
    int lastKeyPressed;
    const int speed = 5; // Speed of notes falling, remember the speed is the px per frame

    const int perFrame = 16; // 60 FPS, remember the perFrame is the 1 frame is 16ms, means it is the ms/p
private slots:
    void continueGame();
    void restartGame();
    void exitGame();

};

#endif // PLAYWINDOW_H
