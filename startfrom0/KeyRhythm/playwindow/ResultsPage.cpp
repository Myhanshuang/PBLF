#include "ResultsPage.h"
#include <QPainter>

ResultsPage::ResultsPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);  // Set size for the results page
}

ResultsPage::~ResultsPage() {}

void ResultsPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw some simple result text
    painter.drawText(300, 300, "Game Over! Your Score: 1000");
}

void ResultsPage::displayResults()
{
    // You could display score or any other game-over information here
}
