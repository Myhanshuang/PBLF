#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFont>
#include <QPainter>
#include <QPixmap>

class ResultPage : public QWidget {
    Q_OBJECT

public:
    explicit ResultPage(QWidget* parent = nullptr);
    void setResults(int perfectNum, int greatNum, int goodNum, int badNum, int missNum, float accuracy, int maxCombo, const QString& finalEvaluation,const int score, const QString& songTitle, const QString& rscImagePath);

signals:
    void requestToRestartGame();
    void requestToHomePage();

public slots:
    void onShowResults();

private slots:
    void onRetryGame();
    void onExitToHome();

private:
    // Widgets for the result page
    int windowWidth;
    int windowHeight;
    QLabel* score;
    QLabel* titleLabel;
    QLabel* songTitleLabel;
    QLabel* finalEvaluationLabel;
    QLabel* perfectLabel;
    QLabel* greatLabel;
    QLabel* goodLabel;
    QLabel* badLabel;
    QLabel* missLabel;
    QLabel* accuracyLabel;
    QLabel* maxComboLabel;
    QPushButton* retryButton;
    QPushButton* exitButton;

    QGraphicsView* imageView;
    QGraphicsScene* scene;
};

#endif // RESULTPAGE_H
