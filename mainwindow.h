#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ChartAnalyzer.h"
#include "chartRenderer.h"
#include <QtMultimedia/QtMultimedia>
#include <QTimer>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // send to checker
    struct Information{
        int time;
        int key;

    };
    //recv from checker
    struct Result{
        int colume;
        short status;
        int scores;
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);
public slots :
    void updateFrame(int time);
    void onAudioFinished();

private:
    Ui::MainWindow *ui;
    void paintChart();
    void startPlayback();
    void updateFrame();
    Result checkNote(Information messageSendToChecker);
    ChartRenderer *chartRenderer;
    Chart *chartAnalyzer;
    QTimer *timer;
    QMediaPlayer *audioPlayer;
    qint64 startTime;
    Chart currentChart;
    QString chartFileName;
};
#endif // MAINWINDOW_H
