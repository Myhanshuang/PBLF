#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtimezone.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartRenderer(new ChartRenderer(this))
    , chartAnalyzer(new Chart())
    , timer(new QTimer(this))
    , audioPlayer(new QMediaPlayer(this))
{
    // Set up the UI
    ui->setupUi(this);
    //设置中央窗口的部件为 chartRenderer
    setCentralWidget(chartRenderer);
    // Set up the timer, as the time goes by, the frame will be updated
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    // Set up the audio player, when the audio is finished, the frame will be updated
    connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onAudioFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintChart()
{
    currentChart = getChart(chartFileName.toStdString().c_str());
    
    // Set the start time to the current time minus 3 seconds
    startTime = QTime::currentTime().msecsSinceStartOfDay() - 3000;

    //play the audio
    //chart music lacked
    audioPlayer ->setSource(QUrl::fromLocalFile("../test/1.mp3"));
    audioPlayer -> play();

    //timer start and update per second 
    timer -> start(1);


    //now go to paint it 
    /*
    first caculate every note's position
    without doubt it should be with two factors:
    first is the time it should appear 
    the second is the column it should appear
    and here we could define a window that shows from the current time to the future based on the speed the note run 
    the caculation is like this:

    define the windowSize = 10000pts
    the speed the note run = 1.0f pts/ms
    and add a variable which multiply the speed that decide the windowEndTime

    windowEndTime = currentTime + windowSize / speed the note run 

    so now we get a whole window that slips on the time axis and only in the window the notes should be painted 
    using a timer to track the current time and display the notes in the window, and the timer should be the same as the one in audio player and the note checker
    as the timer running per 1ms, the window move and we repaint it in the paintEvent function.
    but it is easy to find that we can change the efficiency of the program by pre-caculation of the notes' positon and store it in a class
    so here we could init the class and just read it when play.

    however , there should be a factor that depends on the note checker return to hide it at the time the keybord was pressed 
    don't forget to add the keybord event to the running function.
    */
}

void MainWindow::updateFrame()
{
    chartRenderer -> update();
}

void MainWindow::startPlayback()
{
    paintChart();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{c
    // Get the key that was pressed
    int key = event -> key();
    // Check the note
    Information messageSendToChecker;
    //send to checker
    Result result = checkNote(messageSendToChecker);
    // If the note was hit, update the score
    switch(result.status)
    {
        // perfect
        case 1:
            // Update the score
            break;
        // great
        case 2:
            // Update the score
            break;
        // good
        case 3:
            // Update the score
            break;
        // miss
        case 4:
            // Update the score
            break;
        //empty
        case 5:
            // Update the score
            break;
        default:
            break;
    }
    return ;
}
MainWindow::Result MainWindow::checkNote(Information messageSendToChecker)
{
    // Check the note
    // return ;
}

void MainWindow::onAudioFinished()
{
    if (audioPlayer->mediaStatus() == QMediaPlayer::EndOfMedia) {
        // 切换到另一个页面
        // 例如：ui->stackedWidget->setCurrentIndex(1);
    }
}
