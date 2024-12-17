#include "PlayWindow.h"
#include "Chart.h"
#include "ClickablePixmapItem.h"


PlayWindow::PlayWindow(QWidget* parent)
    : QMainWindow(parent), scene(new QGraphicsScene(this)), view(new QGraphicsView(scene, this)), gameTimer(new QTimer(this)), gameTime(0) {

    keyToColumn[Qt::Key_D] = 1; // Column 1
    keyToColumn[Qt::Key_F] = 2; // Column 2
    keyToColumn[Qt::Key_J] = 3; // Column 3
    keyToColumn[Qt::Key_K] = 4; // Column 4

    setupUI();


    connect(gameTimer, &QTimer::timeout, this, &PlayWindow::updateGame);

}

PlayWindow::~PlayWindow() {
    delete scene;
    delete view;
}


QPixmap PlayWindow::createDarkenedImage(const QString &imagePath, qreal opacity) {//finished
    // Load the original image
    QPixmap original(imagePath);

    // Check if the image was loaded successfully
    if (original.isNull()) {
        qWarning("Failed to load image");
        return QPixmap();
    }

    // Create a pixmap with the same size as the original
    QPixmap darkened(original.size());
    darkened.fill(Qt::transparent); // Start with a transparent pixmap

    // Use QPainter to draw the original image and overlay
    QPainter painter(&darkened);
    painter.drawPixmap(0, 0, original); // Draw the original image

    // Set the color and opacity of the overlay
    QColor overlayColor(0, 0, 0, static_cast<int>(opacity * 255)); // Black with opacity
    painter.fillRect(darkened.rect(), overlayColor); // Draw the overlay

    painter.end();
    return darkened;
}

QPixmap PlayWindow::setOpacityImage(const QString &imagePath, qreal opacity) {//finished
    // Load the original image
    QPixmap original(imagePath);

    // Check if the image was loaded successfully
    if (original.isNull()) {
        qWarning("Failed to load image");
        return QPixmap();
    }

    // Convert the QPixmap to a QImage to manipulate the pixels
    QImage image = original.toImage();

    // Check if the image has an alpha channel
    if (!image.hasAlphaChannel()) {
        // If not, create one
        image = image.convertToFormat(QImage::Format_ARGB32);
    }

    // Create a new QImage with the same size as the original
    QImage darkened(image.size(), QImage::Format_ARGB32);
    darkened.fill(Qt::transparent); // Start with a transparent image

    // Use QPainter to draw the original image and adjust opacity
    QPainter painter(&darkened);
    painter.drawImage(0, 0, image); // Draw the original image

    // Adjust the opacity of the image
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.setOpacity(opacity);
    painter.drawImage(0, 0, image); // Draw the original image again with reduced opacity

    painter.end();
    return QPixmap::fromImage(darkened);
}

void PlayWindow::PlayWindow::setupUI() { //finished
    setWindowTitle("Just enjoy");
    // x * y (size)
    const int windowWidth = 1280;
    const int windowHeight = 720;

    resize(windowWidth, windowHeight);

    //QGraphicsScene 使用自己的坐标系，该坐标系独立于 this->height() 或 this->width() 等小部件尺寸。当我们向场景添加一个项目时，它的位置是相对于场景的原点 (0,0) 的，而不是窗口或 QGraphicsView 。
    scene->setSceneRect(0, 0, windowWidth, windowHeight);

    view->setScene(scene);
    setCentralWidget(view);
    //drawchannels();
    drawChannels();

    //background here

    // remember to give me the chart's src path
    fileSource = ":/Chart/testChart";
    auto backgroundSource = fileSource;
    QPixmap darkenTheBackground = createDarkenedImage(backgroundSource.append(".png"), 0.4);
    if (!darkenTheBackground.isNull()) {
        auto* bgItem = new QGraphicsPixmapItem(darkenTheBackground);
        bgItem->setZValue(-1); // Ensure the background is below other items
        scene->addItem(bgItem);

        // Optionally scale to fit the scene
        bgItem->setPixmap(darkenTheBackground.scaled(scene->width(), scene->height(), Qt::KeepAspectRatioByExpanding));
    } else {
        qDebug() << "Failed to load background image!";
    }

    //ensure the media
    auto musicSource = fileSource;
    musicSource.append(".mp3");

    // Checker Line
    checkerLine = new QGraphicsLineItem(0, scene->height() * 3 / 4, scene->width(), scene->height() * 3 / 4);
    checkerLine->setPen(QPen(Qt::yellow, 4));
    scene->addItem(checkerLine);


    // Add the stats label
    statsLabel = new QLabel(this);
    statsLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0.7); color: white; padding: 10px; border-radius: 5px;");
    statsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    statsLabel->setText("Accuracy: 0\nScore: 0\nCombo: 100%");
    statsLabel->setGeometry(scene->width() * 2 / 3 + 20, 10, 150, 100); // Position it to the right of channels

    // Load custom font
    // int fontId = QFontDatabase::addApplicationFont(":/fonts/custom_font.ttf"); // Replace with your font file
    // if (fontId != -1) {
    //     QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    //     statsLabel->setFont(QFont(fontFamily, 16));
    // } else {
    //     statsLabel->setFont(QFont("Arial", 16)); // Fallback font
    // }

    //pause menu
    gameTimer = new QTimer(this);
    // Pause menu initialization
    pauseMenuBackground = nullptr;
    continueButton = nullptr;
    restartButton = nullptr;
    exitButton = nullptr;
    isPaused = false;

    loadChart();
}

void PlayWindow::showPauseMenu() { // finished
    // Stop the game
    gameTimer->stop();
    isPaused = true;

    // Add a semi-transparent background
    pauseMenuBackground = new QGraphicsRectItem(0, 0, scene->width(), scene->height());
    pauseMenuBackground->setBrush(QBrush(QColor(0, 0, 0, 150))); // Semi-transparent black
    scene->addItem(pauseMenuBackground);

    // Add buttons
    continueButton = new ClickablePixmapItem(QPixmap(":/img/continue.png"));
    restartButton = new ClickablePixmapItem(QPixmap(":/img/restart.png"));
    exitButton = new ClickablePixmapItem(QPixmap(":/img/exit.png"));

    // Position buttons in the center
    int centerX = scene->width() / 2;
    int centerY = scene->height() / 2;
    QSize buttonSize(scene->width() / 5, scene->height() / 10);
    continueButton->setPos(centerX , centerY - 100);
    restartButton->setPos(centerX , centerY - 33);
    exitButton->setPos(centerX , centerY + 33);

    // Add buttons to the scene
    scene->addItem(continueButton);
    scene->addItem(restartButton);
    scene->addItem(exitButton);

    // Connect button clicks to slots
    connect(continueButton, &ClickablePixmapItem::clicked, this, &PlayWindow::continueGame);
    connect(restartButton, &ClickablePixmapItem::clicked, this, &PlayWindow::restartGame);
    connect(exitButton, &ClickablePixmapItem::clicked, this, &PlayWindow::exitGame);
}

void PlayWindow::hidePauseMenu() { // finished
    // Remove pause menu items
    if (pauseMenuBackground) {
        scene->removeItem(pauseMenuBackground);
        delete pauseMenuBackground;
        pauseMenuBackground = nullptr;
    }
    if (continueButton) {
        scene->removeItem(continueButton);
        delete continueButton;
        continueButton = nullptr;
    }
    if (restartButton) {
        scene->removeItem(restartButton);
        delete restartButton;
        restartButton = nullptr;
    }
    if (exitButton) {
        scene->removeItem(exitButton);
        delete exitButton;
        exitButton = nullptr;
    }

    // Resume the game
    isPaused = false;
    //wait 1000ms after pause to start
    Sleep(1000);
    gameTimer->start();
}

void PlayWindow::resizeEvent(QResizeEvent* event) {//finished
    QMainWindow::resizeEvent(event);

    // Update the scene size to match the new window size
    scene->setSceneRect(0, 0, this->width(), this->height());

    //pause menu
    if (isPaused) {
        // Update background size
        pauseMenuBackground->setRect(0, 0, scene->width(), scene->height());

        // Update button sizes
        QSize buttonSize(scene->width() / 5, scene->height() / 10);
        if (continueButton) {
            continueButton->resizeTo(buttonSize);
            continueButton->setPos(scene->width() / 2 , scene->height() / 2 - 100);
        }
        if (restartButton) {
            restartButton->resizeTo(buttonSize);
            restartButton->setPos(scene->width() / 2 , scene->height() / 2 - 33);
        }
        if (exitButton) {
            exitButton->resizeTo(buttonSize);
            exitButton->setPos(scene->width() / 2 , scene->height() / 2 + 33);
        }
    }

    // Update stats label position and size
    int labelX = scene->width() * 4 / 5;
    int labelY = 8;
    int labelWidth = scene->width() / 8;
    int labelHeight = scene->height() / 8;

    statsLabel->setGeometry(labelX, labelY, labelWidth, labelHeight);

    // Update checker line position
    if (!scene->items().contains(checkerLine)) {
        // qDebug() << "CheckerLine is missing! Re-adding it.";
        int checkerLineY = scene->height() * 3 / 4;
        checkerLine = new QGraphicsLineItem(0, checkerLineY, scene->width(), checkerLineY);
        checkerLine->setPen(QPen(Qt::darkGray, 4));
        scene->addItem(checkerLine);
    } else {
        // Update checker line position
        int checkerLineY = scene->height() * 4 / 5;
        checkerLine->setLine(0, checkerLineY, scene->width(), checkerLineY);
    }

    // Update channel positions
    drawChannels();
}

void PlayWindow::updateStats() {
    // Update combo, score, and accuracy dynamically
    statsLabel->setText(QString("Combo: %1\nScore: %2\nAccuracy: %3%")
                            .arg(QString::number(accuracy, 'f', 2)
                            .arg(score)
                            .arg(combo)
                            ));
}

void PlayWindow::drawChannels() {//finished

    // Clear existing channels from the scene
    for (auto* item : scene->items()) {
        auto* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
        if (lineItem && lineItem != checkerLine) {//never forget the checkerline
            scene->removeItem(lineItem);
            delete lineItem;
        }
    }

    if (currentChart.Column <= 0) {
        qDebug() << "Invalid number of columns in chart!";
        return;
    }

    // Determine the total channel area (1/3 of the scene width)
    int sceneWidth = scene->width();
    int sceneHeight = scene->height();
    int channelAreaWidth = sceneWidth * 1 / 3;
    int channelStartX = (sceneWidth - channelAreaWidth) / 2; // Center the channels

    // Width of each channel
    int channelWidth = channelAreaWidth / currentChart.Column;

    // Draw lines for each channel

    for (int i = 0; i <= currentChart.Column; ++i) { // i <= Column ensures the outermost lines are drawn
        int x = channelStartX + i * channelWidth;

        // Draw vertical line
        auto* line = new QGraphicsLineItem(x, 0, x, sceneHeight);
        line->setPen(QPen(Qt::white, 4));
        scene->addItem(line);

        // Optional: Replace line with image

        // QPixmap lineImage(":/images/line.png");
        // if (!lineImage.isNull()) {
        //     auto* lineItem = new QGraphicsPixmapItem(lineImage);
        //     lineItem->setPos(x - lineImage.width() / 2, 0); // Center the image on the line
        //     scene->addItem(lineItem);
        // }
    }

}

void PlayWindow::loadChart() { // finished
    // Open the chart file
    auto chartSource = fileSource;
    chartSource.append(".mc");
    QFile chartfile(chartSource);
    if (!chartfile.open(QIODevice::ReadOnly)) {
        qDebug() << "Errorqfile: Unable to open file" << chartSource;
        return;
    }
    //transform the QString to the standard form and to open the file
    std::string s = "../../" + chartSource.right(chartSource.size() - 1).toStdString();
    FILE* chartFile = fopen(s.c_str(), "r");
    if (chartFile == nullptr) {
       qDebug() << "Error: Unable to open file" << chartSource;
       return;
    }

    try {
        // Parse the chart using the provided getChart function
        getChart(chartFile, currentChart);

        // Log successful chart loading
        qDebug() << "Chart loaded successfully!";

    } catch (const ChartError& e) {
        // Handle chart parsing errors
        qDebug() << "Chart parsing failed:" << e.what();
    }

    // Close the file
    //QFile auto close
    fclose(chartFile);
}

void PlayWindow::startGame() {// maybe bug and error here

    //reset the data

    gameTime = 0;
    combo = 0;
    score = 0;
    accuracy = 100.0;
    scene -> clear();

    auto backgroundSource = fileSource;
    QPixmap darkenTheBackground = setOpacityImage(backgroundSource.append(".png"), 0.5);
    if (!darkenTheBackground.isNull()) {
        auto* bgItem = new QGraphicsPixmapItem(darkenTheBackground);
        bgItem->setZValue(-1); // Ensure the background is below other items
        scene->addItem(bgItem);

        // Optionally scale to fit the scene
        bgItem->setPixmap(darkenTheBackground.scaled(scene->width(), scene->height(), Qt::KeepAspectRatioByExpanding));
    } else {
       qDebug() << "Failed to load background image!";
    }

    // Re-add the checker line
    int checkerLineY = scene->height() * 3 / 4;
    checkerLine = new QGraphicsLineItem(0, checkerLineY, scene->width(), checkerLineY);
    checkerLine->setPen(QPen(Qt::darkGray, 4));
    scene->addItem(checkerLine);

    // Re-add the label box (stats display)
    statsLabel->setGeometry(scene->width() * 4 / 5, 8, scene->width() / 8, scene->height() / 8);
    statsLabel->setText("Combo: 0\nScore: 0\nAccuracy: 100%");
    // Redraw the channels
    drawChannels();
    // Prepare the chart data
    if (!currentChart.ChartHead) {
        qDebug() << "Error: Chart data is empty. Cannot start game.";
        return;
    }

    //wait 2 seconds before starting the game

    Sleep(2000);

    // Load the first notes based on the chart
    spawnNotes();

    // 4. Start the game timer
    gameTimer->start(perFrame); // ~60 FPS
    qDebug() << "Game started!";
}

void PlayWindow::updateGame() {
    // gameTime += perFrame; // Update game time (milliseconds)
    // spawnNotes();

    // // Move notes down
    // for (auto note : notes) {
    //     note->moveBy(0, speed); // Adjust speed as needed
    //     if (note->y() > scene -> height()) {
    //         scene->removeItem(note); // Remove missed notes
    //         delete note ;

    //         combo = 0;
    //         updateStats();
    //     }
    // }

    // // Check for collisions (when notes reach the checker line)
    // //This is handled by the checkCollisions() function, which checks if any notes have been hit by the player.
    // checkCollisions();//unfinished

    // // Spawn new notes based on the chart
    // spawnNotes();
}

void PlayWindow::checkCollisions() {// by GPT all
    // // Loop through all notes and check if they reach the checker line
    // for (auto* note : notes) {
    //     // Check if note is within the timing window (near checker line)
    //     if (note->y() >= scene->height() * 3 / 4 - 10 && note->y() <= scene->height() * 3 / 4 + 10) {
    //         // Find out which column this note is in (based on x position)
    //         int noteColumn = note->x() / (scene->width() / currentChart.Column); // Adjust if needed

    //         // Check if the player pressed the correct key for this column
    //         if (keyToColumn.contains(lastKeyPressed) && keyToColumn[lastKeyPressed] == noteColumn) {
    //             // The player hit the note
    //             scene->removeItem(note);
    //             notes.removeOne(note);
    //             delete note;

    //             // Update combo, score, and accuracy
    //             combo++;
    //             score += 100; // Adjust based on game rules
    //             accuracy = (accuracy * (score / 100 - 1) + 100) / (score / 100);

    //             // Update stats
    //             updateStats();
    //             return;
    //         }
    //     }
    // }
}

void PlayWindow::spawnNotes() {// maybe error bug here
    auto* measure = currentChart.ChartHead;

    double distance = scene->height() - checkerLine->y();// the distance the note need to pass
    double timeToPassDistance = distance / speed * perFrame;//the time the note to move to the checkerline
    double paintTime = gameTime - timeToPassDistance;// the time to paint the note
    while (measure && paintTime >= measure -> timeStamp) {

    //     for (int i = 0; i < currentChart.Column; ++i) {
    //         if (measure->Bar[i] > 0) { // A note exists in this column
    //             if(measure->Bar[i] == 1)
    //             {
    //                 // Create a falling note
    //                 int channelAreaWidth = scene -> width() * 1 / 3;
    //                 //need to change the form of the note]
    //                 int channelWidth = channelAreaWidth / currentChart.Column;
    //                 int channelStartX = (scene->width() - channelAreaWidth) / 2; // Center the channels
    //                 int x = channelStartX + i * channelWidth;
    //                 QGraphicsRectItem* note = new QGraphicsRectItem(x, 0, channelWidth, 20);
    //                 note->setBrush(Qt::green); // Example color
    //                 scene->addItem(note);
    //                 notes.append(note); // Store in notes vector for updates
    //             }
    //             else // hold
    //             {
    //                 // // Create a falling note
    //                 // int channelAreaWidth = scene -> width() * 1 / 3;
    //                 // //need to change the form of the note]
    //                 // int channelWidth = channelAreaWidth / currentChart.Column;
    //                 // int channelStartX = (scene->width() - channelAreaWidth) / 2; // Center the channels
    //                 // int x = channelStartX + i * channelWidth;
    //                 // int holdheight = (measure->timeTable[i] - measure->timeStamp) * speed / perFrame ;
    //                 // QGraphicsRectItem* note = new QGraphicsRectItem(x, 0, channelWidth, holdheight);
    //                 // note->setBrush(Qt::yellow); // Example color
    //                 // scene->addItem(note);
    //                 // notes.append(note); // Store in notes vector for updates
    //             }
    //         }
    //     }

    //     // Move to the next measure
    //     currentChart.ChartHead = measure->NxtMea;
    //     measure = measure->NxtMea;
    }
}

void PlayWindow::keyPressEvent(QKeyEvent* event) {// cooperation with wdx
    if (event->key() == Qt::Key_Escape) {
        if (isPaused) {
            hidePauseMenu();
        } else {
            showPauseMenu();
        }
    }

    //GPT ALL
    // Store the last key pressed for later collision detection
    // if (keyToColumn.contains(event->key())) { // 1 2 3 4 all right
    //     lastKeyPressed = event->key();
    //     checkCollisions(); // Check if the player hit any notes
    // }
}

void PlayWindow::restartGame() {// actually bug here
    hidePauseMenu();
    // Add logic to restart the game
    qDebug() << "Restarting game...";

    startGame();
}

void PlayWindow::continueGame() {//finishd
    hidePauseMenu();

}

void PlayWindow::exitGame() {// cope with lyjy
    qDebug() << "Exiting game...";
    //turn to home page
    QApplication::quit();
}
void PlayWindow::addFileSource(QString path)
{
    fileSource = path;
}
