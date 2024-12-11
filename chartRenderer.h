#ifndef CHARTRENDERER_H
#define CHARTRENDERER_H

#include <QWidget>
#include <QTimer>
#include "chartanalyzer.h"

class ChartRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit ChartRenderer(QWidget *parent = nullptr);
    void setChartData(const Chart &chartData);
    void startPlayback();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateFrame();

private:
    Chart chartData;
    QTimer *timer;
    qint64 startTime;
};

#endif // CHARTRENDERER_H
