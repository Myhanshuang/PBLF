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
#include <QtMultimedia/QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QGraphicsProxyWidget>

#ifndef offsetFromLineToNote//(liney, notey)
#define offsetFromLineToNote(liney, notey) abs(abs(notey - liney)* perFrame / speed - gameTime)
#endif

// Replace QGraphicsPixmapItem* with ClickablePixmapItem*

extern int KeyCode[9];
extern int MaxOffset[8];

class PlayWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit PlayWindow(QWidget* parent = nullptr);
    ~PlayWindow();
    Chart currentChart;
    void loadChart();
    void startGame();
    void addFileSource(QString path);
    QString readFileSource();

signals:
    void requestToHomePage();
    void requestToResultPage();
    void requestToRestartGame();

public slots:
    void start();
    void restartGame();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event)override;
private:
    void debug();
    Chart::ChartAct gameEnd();
    void setupUI();
    void updateGame();
    void spawnNotes();
    void checkJudgment();
    void updateStats();
    void showPauseMenu();
    void hidePauseMenu();
    void drawChannels();
    void checkCollisions();
    void initGameState();
    void addWaiting(QGraphicsTextItem *waiting);
    void removeWaiting(QGraphicsTextItem *waiting);
    QPixmap setOpacityImage(const QString &imagePath, qreal opacity);
    QPixmap createDarkenedImage(const QString& imagePath, qreal opacity = 0.5);
    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsLineItem* checkerLine;
    QTimer* gameTimer;
    QLabel* statsLabel;
    Chart stasticChart;

    QString fileSource;
    // Chart currentChart, stasticChart;
    QMediaPlayer* musicPlayer;

    struct TempStoreInScene{
        QGraphicsRectItem* item;
        bool ifHold;
        bool holdJudge = 0;
        double length;
        int column;
    };

    QVector<TempStoreInScene> notes;
    int gameTime;
    int checkerLineHeight;

    // Pause Menu
    QGraphicsRectItem* pauseMenuBackground;
    ClickablePixmapItem* continueButton;
    ClickablePixmapItem* restartButton;
    ClickablePixmapItem* exitButton;
    bool isPaused;
    QMap<int, int> keyToColumn;

    //in game
    int lastKeyPressed;
    const int speed = 5; // Speed of notes falling, remember the speed is the px per frame

    const int perFrame = 8; // 60 FPS, remember the perFrame is the 1 frame is 16ms, means it is the ms/p
private slots:
    void continueGame();

    void exitGame();
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

};

#endif // PLAYWINDOW_H
