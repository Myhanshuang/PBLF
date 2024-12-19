#include "ResultPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <QFont>
#include <QGraphicsItem>
#include <QCoreApplication>

ResultPage::ResultPage(QWidget* parent)
    : QWidget(parent),
    titleLabel(new QLabel(this)),
    songTitleLabel(new QLabel(this)),
    finalEvaluationLabel(new QLabel(this)),
    perfectLabel(new QLabel(this)),
    greatLabel(new QLabel(this)),
    goodLabel(new QLabel(this)),
    badLabel(new QLabel(this)),
    missLabel(new QLabel(this)),
    accuracyLabel(new QLabel(this)),
    maxComboLabel(new QLabel(this)),
    retryButton(new QPushButton(this)),
    exitButton(new QPushButton(this)),
    imageView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    score(new QLabel(this)){

    // Apply shadow effect
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(3, 3);
    // shadowEffect->setBlurRadius(1);
    shadowEffect->setColor(Qt::darkGray);
    setGraphicsEffect(shadowEffect);
    // Set up main layout


    windowWidth = 1280;
    windowHeight = 720;

    resize(windowWidth, windowHeight);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* contentLayout = new QHBoxLayout();

    // Left side: image and song title
    QVBoxLayout* leftLayout = new QVBoxLayout();
    songTitleLabel->setAlignment(Qt::AlignCenter);
    QFont songTitleFont;
    songTitleFont.setPointSize(40);
    songTitleFont.setBold(true);
    songTitleLabel->setFont(songTitleFont);
    songTitleLabel->setStyleSheet("color: #87CEEB;");

    // Set up image scene
    imageView->setScene(scene);
    imageView->setRenderHint(QPainter::Antialiasing);

    leftLayout->addWidget(songTitleLabel);
    leftLayout->addWidget(imageView);

    // Right side: final evaluation and stats
    QFont finalEvaluationFont;
    finalEvaluationFont.setPointSize(80);
    finalEvaluationFont.setBold(true);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    finalEvaluationLabel->setAlignment(Qt::AlignCenter);
    finalEvaluationLabel->setFont(finalEvaluationFont);

    //set score
    QFont scoreFont;
    scoreFont.setPointSize(35);
    score->setAlignment(Qt::AlignCenter);
    score->setFont(scoreFont);
    score->setStyleSheet("color: white;");
    // score->setStyleSheet("color: black;");

    // Stats labels (Perfect, Great, etc.)
    QFont statsFont;
    statsFont.setPointSize(28);
    perfectLabel->setFont(statsFont);
    greatLabel->setFont(statsFont);
    goodLabel->setFont(statsFont);
    badLabel->setFont(statsFont);
    missLabel->setFont(statsFont);
    accuracyLabel->setFont(statsFont);
    maxComboLabel->setFont(statsFont);

    perfectLabel->setStyleSheet("color: white;");
    greatLabel->setStyleSheet("color: white;");
    goodLabel->setStyleSheet("color: white;");
    badLabel->setStyleSheet("color: white;");
    missLabel->setStyleSheet("color: white;");
    accuracyLabel->setStyleSheet("color: white;");
    maxComboLabel->setStyleSheet("color: white;");

    // perfectLabel->setStyleSheet("color: black;");
    // greatLabel->setStyleSheet("color: black;");
    // goodLabel->setStyleSheet("color: black;");
    // badLabel->setStyleSheet("color: black;");
    // missLabel->setStyleSheet("color: black;");
    // accuracyLabel->setStyleSheet("color: black;");
    // maxComboLabel->setStyleSheet("color: black;");


    rightLayout->addWidget(finalEvaluationLabel);
    rightLayout->addWidget(score);
    rightLayout->addWidget(perfectLabel);
    rightLayout->setAlignment(perfectLabel, Qt::AlignCenter);
    rightLayout->addWidget(greatLabel);
    rightLayout->setAlignment(greatLabel, Qt::AlignCenter);
    rightLayout->addWidget(goodLabel);
    rightLayout->setAlignment(goodLabel, Qt::AlignCenter);
    rightLayout->addWidget(badLabel);
    rightLayout->setAlignment(badLabel, Qt::AlignCenter);
    rightLayout->addWidget(missLabel);
    rightLayout->setAlignment(missLabel, Qt::AlignCenter);
    rightLayout->addWidget(accuracyLabel);
    rightLayout->setAlignment(accuracyLabel, Qt::AlignCenter);
    rightLayout->addWidget(maxComboLabel);
    rightLayout->setAlignment(maxComboLabel, Qt::AlignCenter);

    // Buttons (Retry and Back to Home)
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    //retry button
    retryButton->setStyleSheet("background-image: url(:/img/restart.png);"
                               "border: none; padding: 0;" );  // No border, no padding, just image
    retryButton->setFixedSize(50, 50);  // Set the size for the button
    buttonLayout->addWidget(retryButton);

    //exit button
    exitButton->setStyleSheet("background-image: url(:/img/exit.png);"
                              "border: none; padding: 0;");  // No border, no padding, just image
    exitButton->setFixedSize(50, 50);  // Set the size for the button
    buttonLayout->addWidget(exitButton);


    rightLayout->addLayout(buttonLayout);



    // Combine left and right layout
    contentLayout->addLayout(leftLayout, 2); // 2 parts for left side
    contentLayout->addLayout(rightLayout, 1); // 1 part for right side

    mainLayout->addLayout(contentLayout);

    // Connect signals and slots
    connect(retryButton, &QPushButton::clicked, this, &ResultPage::onRetryGame);
    connect(exitButton, &QPushButton::clicked, this, &ResultPage::onExitToHome);

}

void ResultPage::setResults(int perfectNum, int greatNum, int goodNum, int badNum, int missNum, float accuracy, int maxCombo, const QString& finalEvaluation,const int scoreNum, const QString& songTitle, const QString& rscImagePath) {

    // Load the background image and adjust it
    QPixmap image(rscImagePath);
    if (!image.isNull()) {
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
        pixmapItem->setPixmap(image.scaled(windowWidth * 3 / 5, windowHeight * 3 / 5, Qt::KeepAspectRatioByExpanding));
        scene->clear();  // Clear the previous items
        scene->addItem(pixmapItem);
    }
    else {
        qDebug() << "Failed to load image: " << rscImagePath;
    }
    // Set final evaluation
    switch(finalEvaluation.toStdString().c_str()[0])
    {
        case 'S':
            finalEvaluationLabel->setStyleSheet("color: gold;");
            break;
        case 'A':
            finalEvaluationLabel->setStyleSheet("color: #800080;");
            break;
        case 'B':
            finalEvaluationLabel->setStyleSheet("color: blue;");
            break;
        case 'C':
            finalEvaluationLabel->setStyleSheet("color: silver;");
            break;
        case 'F':
            finalEvaluationLabel->setStyleSheet("color: white;");
            break;
    }
    // Set song title
    songTitleLabel->setText(songTitle);

    finalEvaluationLabel->setText(finalEvaluation);
    score->setText(QString("Score: %1").arg(scoreNum));

    // Set stats labels
    perfectLabel->setText(QString("Perfect: %1").arg(perfectNum));
    greatLabel->setText(QString("Great: %1").arg(greatNum));
    goodLabel->setText(QString("Good: %1").arg(goodNum));
    badLabel->setText(QString("Bad: %1").arg(badNum));
    missLabel->setText(QString("Miss: %1").arg(missNum));
    accuracyLabel->setText(QString("Accuracy: %1\%").arg(accuracy, 0, 'f', 2));
    maxComboLabel->setText(QString("Max Combo: %1").arg(maxCombo));

}

void ResultPage::onShowResults()
{
    //load the result
    this->show();
}

void ResultPage::onRetryGame() {
    qDebug() << "Retry game";
    emit requestToRestartGame();
}

void ResultPage::onExitToHome() {
    qDebug() << "Exit to home";
    emit requestToHomePage();
}

