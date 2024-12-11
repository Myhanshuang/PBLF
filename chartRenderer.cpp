#include "chartrenderer.h"
#include <QPainter>
#include <QTime>
#include "ChartAnalyzer.h"
ChartRenderer::ChartRenderer(QWidget *parent) : QWidget(parent), timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &ChartRenderer::updateFrame);
}

void ChartRenderer::setChartData(const Chart &chartData)
{
    this->chartData = chartData;
    update(); // 触发重绘
}

void ChartRenderer::startPlayback()
{
    startTime = QTime::currentTime().msecsSinceStartOfDay();
    timer->start(16); // 大约60 FPS
}

void ChartRenderer::updateFrame()
{
    update(); // 触发重绘
}

void ChartRenderer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    qint64 currentTime = QTime::currentTime().msecsSinceStartOfDay();
    qint64 elapsedTime = currentTime - startTime;

    // 遍历链表中的每个 Measure 对象
    Chart::Measure *currentMeasure = chartData.ChartHead;

    while (currentMeasure != nullptr) {
        // 渲染每个 measure
        for (int i = 0; i < chartData.Column; ++i) {
            int time = currentMeasure->timeTable[i];
            if (time > 0) {
                // 根据时间和列计算绘制位置
                int x = i * 50; // 每列的宽度为 50 像素
                int y = (time - elapsedTime) / 10; // 时间转换为 y 坐标

                if (y >= 0 && y < height()) {
                    painter.drawRect(x, y, 40, 10); // 绘制一个矩形表示音符
                }
            }
        }

        currentMeasure = currentMeasure->NxtMea;
    }
}
