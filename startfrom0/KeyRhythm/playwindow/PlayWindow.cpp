#include "PlayWindow.h"
#include <QPainter>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QTime>
#include <QDebug>

PlayWindow::PlayWindow(QWidget *parent)
    : QWidget(parent),
    gameTimer(new QTimer(this))
{
    // Set the game window size and other properties
    setFixedSize(800, 600);
    gameTimer->setInterval(16);  // ~60 FPS
    connect(gameTimer, &QTimer::timeout, this, &PlayWindow::timerEvent);
}

PlayWindow::~PlayWindow() {
    delete gameTimer;
}

void PlayWindow::loadChart(const Chart& chart) {
    currentChart = chart;
    elapsedTime = 0;
    currentMeasureIndex = 0;
    results.clear();
}

void PlayWindow::startGame() {
    gameRunning = true;
    gameTimer->start();
}

void PlayWindow::stopGame() {
    gameRunning = false;
    gameTimer->stop();
}

void PlayWindow::paintEvent(QPaintEvent *event) {
    if (!gameRunning) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the notes
    drawNotes(painter);

    // Optionally, draw more UI components, like score and health
}

void PlayWindow::keyPressEvent(QKeyEvent *event) {
    if (gameRunning) {
        // Handle key press (e.g., a note hit)
        checkNoteHit();
    }
}

void PlayWindow::keyReleaseEvent(QKeyEvent *event) {
    // Handle key release (if needed)
}

void PlayWindow::timerEvent(QTimerEvent *event) {
    if (gameRunning) {
        processGameLogic();
        update();  // Trigger repaint
    }
}

void PlayWindow::processGameLogic() {
    // Increment elapsed time
    elapsedTime++;

    // Check if a note has reached the judgment zone
    // This requires you to convert time to a note's position on the screen
    // and check if a key has been pressed within the timing window
    checkNoteHit();
}

void PlayWindow::checkNoteHit() {
    // Here, you should calculate whether the player pressed the correct key at the correct time
    // Compare the current time with the timestamp of the next note in the chart
    // Use the `BeatToTime()` macro to calculate the exact time of the note
}

void PlayWindow::drawNotes(QPainter& painter) {
    // Draw the notes falling down the screen, according to the chart's beat timing and the elapsed time
    painter.setBrush(Qt::red);
    painter.setPen(Qt::black);

    // Example: Draw a circle for each note (you need to draw based on the note's position in time)
    for (int i = 0; i < currentChart.NoteCount; i++) {
        // Calculate the position of each note based on time (e.g., falling down based on BPM)
        int noteY = 100 + (elapsedTime - currentChart.ChartHead->timeStamp) / 10;
        painter.drawEllipse(400, noteY, 30, 30);
    }
}

void PlayWindow::calculateScore() {
    // Calculate score based on the player's accuracy in hitting notes
    // You can use a scoring system like "Perfect", "Good", "Miss", etc.
}
