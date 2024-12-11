#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <QWidget>

class ResultsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResultsPage(QWidget *parent = nullptr);
    ~ResultsPage();

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    void displayResults();
};

#endif // RESULTSPAGE_H
