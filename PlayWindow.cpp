#include "PlayWindow.h"

#define PERFECT (16)
#define GREATE (64 - 3 * 9)
#define GOOD (97 - 3 * 9)
#define BAD (127 - 3 * 9)
#define MISS (151 - 3 * 9)

PlayWindow::PlayWindow(QWidget* parent)
    : QMainWindow(parent), scene(new QGraphicsScene(this)), view(new QGraphicsView(scene, this)), gameTimer(new QTimer(this)), gameTime(0) {//finished
    // Column 1 ~ 4
    keyToColumn[KeyCode[0]] = 1;
    keyToColumn[KeyCode[1]] = 2;
    keyToColumn[KeyCode[2]] = 3;
    keyToColumn[KeyCode[3]] = 4;
    setupUI();
    connect(gameTimer, &QTimer::timeout, this, &PlayWindow::updateGame);
}
/*
 * accept the signal and start the whole game
 */
void PlayWindow::start(){
    // Column 1 ~ 4
    keyToColumn[KeyCode[0]] = 1;
    keyToColumn[KeyCode[1]] = 2;
    keyToColumn[KeyCode[2]] = 3;
    keyToColumn[KeyCode[3]] = 4;
    this->show();
    startGame();
}

PlayWindow::~PlayWindow() {
    delete scene;
    delete view;
}
/*
 * set the background img
 */
QPixmap PlayWindow::createDarkenedImage(const QString &imagePath, qreal opacity) {
    // Load the original image
    QPixmap original(imagePath);

    if (original.isNull()) {
        qWarning("Failed to load image");
        return QPixmap();
    }

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
/*
 * another way to set the opacity
 */
QPixmap PlayWindow::setOpacityImage(const QString &imagePath, qreal opacity) {

    QPixmap original(imagePath);

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
/*
 * draw the UI when new the object
 */
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

    drawChannels();

    //background here
    fileSource = ":/Chart/testChart";
    auto backgroundSource = fileSource;
    QPixmap darkenTheBackground = createDarkenedImage(backgroundSource.append(".png"), 0.4);
    if (!darkenTheBackground.isNull()) {
        auto* bgItem = new QGraphicsPixmapItem(darkenTheBackground);
        bgItem->setZValue(-1);
        scene->addItem(bgItem);
        bgItem->setPixmap(darkenTheBackground.scaled(scene->width(), scene->height(), Qt::KeepAspectRatioByExpanding));
    } else {
        qDebug() << "Failed to load background image!";
    }

    //media
    auto musicSource = fileSource;
    musicSource.append(".mp3");
    musicSource = "qrc" + musicSource;
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setSource(musicSource);
    if(musicPlayer->error() != QMediaPlayer::NoError){
        qDebug() << "Failed to load music!";
    }
    QAudioOutput *audioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(audioOutput);

    // Checker Line
    checkerLineHeight = scene->height() * 3 / 4;
    checkerLine = new QGraphicsLineItem(0, checkerLineHeight, scene->width(), checkerLineHeight);
    checkerLine->setPen(QPen(Qt::yellow, 4));
    scene->addItem(checkerLine);

    // Add the stats label
    statsLabel = new QLabel(this);
    statsLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0.7); color: white; padding: 10px; border-radius: 5px;");
    statsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    statsLabel->setText("Accuracy:100.00\% \nScore: 0\nCombo: 0");
    statsLabel->setGeometry(scene->width() * 2 / 3 + 20, 10, 150, 100); // Position it to the right of channels

    //pause menu
    pauseMenuBackground = nullptr;
    continueButton = nullptr;
    restartButton = nullptr;
    exitButton = nullptr;
    isPaused = false;

    gameTimer = new QTimer(this);

    loadChart();
}
/*
 * the function to show the pausemenu
 * when press the esc
 */
void PlayWindow::showPauseMenu() {
    // Stop the game
    musicPlayer->pause();
    gameTimer->stop();
    isPaused = true;
    gameStatus = 1;
    // Add a semi-transparent background
    pauseMenuBackground = new QGraphicsRectItem(0, 0, scene->width(), scene->height());
    pauseMenuBackground->setBrush(QBrush(QColor(0, 0, 0, 150))); // Semi-transparent black
    pauseMenuBackground->setZValue(99); // Ensure it's above other items
    scene->addItem(pauseMenuBackground);

    // Add buttons
    continueButton = new ClickablePixmapItem(QPixmap(":/img/continue.png"));
    continueButton -> setZValue(100);
    restartButton = new ClickablePixmapItem(QPixmap(":/img/restart.png"));
    restartButton -> setZValue(100);
    exitButton = new ClickablePixmapItem(QPixmap(":/img/exit.png"));
    exitButton -> setZValue(100);

    // Position buttons in the center
    int centerX = scene->width() / 2;
    int centerY = scene->height() / 2;
    QSize buttonSize(scene->width() / 5, scene->height() / 10);
    continueButton->setPos(centerX , centerY - 100);
    restartButton->setPos(centerX , centerY - 33);
    exitButton->setPos(centerX , centerY + 33);

    scene->addItem(continueButton);
    scene->addItem(restartButton);
    scene->addItem(exitButton);

    // Connect button clicks to slots
    connect(continueButton, &ClickablePixmapItem::clicked, this, &PlayWindow::continueGame);
    connect(restartButton, &ClickablePixmapItem::clicked, this, &PlayWindow::restartGame);
    connect(exitButton, &ClickablePixmapItem::clicked, this, &PlayWindow::exitGame);
}

/*
 * at the pausemenu, when press the continue
 * button or press the esc
 */
void PlayWindow::hidePauseMenu() { // finished

    // attention word
    QGraphicsTextItem *waiting;
    waiting = new QGraphicsTextItem("Waiting for 1 second");
    QFont font;
    font.setPointSize(60);
    waiting->setFont(font);
    waiting -> setPos(scene -> width() / 2 - waiting -> boundingRect().width()/2, scene -> height() / 2 - waiting->boundingRect().height() / 2);
    waiting -> setDefaultTextColor(Qt::red);
    waiting -> setZValue(100);
    scene -> addItem(waiting);

    isPaused = false;
    gameStatus = 1;

    //wait 1000ms after pause to start
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 1000) {
        QCoreApplication::processEvents();
    }

    gameStatus = 0;
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


    scene -> removeItem(waiting);
    delete waiting;
    // Resume the game
    gameStatus = 0;
    musicPlayer->play();
    gameTimer->start();

}
/*
 * when the window is resized, the function will be called
 * fix to anyother size
 */
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
        checkerLine = new QGraphicsLineItem(0, checkerLineHeight, scene->width(), checkerLineHeight);
        checkerLine->setPen(QPen(Qt::yellow, 4));
        scene->addItem(checkerLine);
    } else {
        // Update checker line position
        checkerLine->setLine(0, checkerLineHeight, scene->width(), checkerLineHeight);
    }

    // Update channel positions
    drawChannels();
}

/*
 * update the score label
 */
void PlayWindow::updateStats() {

    statsLabel->setText(QString("Accuracy: %1%\nScore: %2\nCombo: %3")
                            .arg(QString::number(currentChart.Acting->Accuracy, 'f', 2))
                            .arg(QString::number(currentChart.Acting->Score))
                            .arg(QString::number(currentChart.Acting->Combo))
                        );

}
/*
 * draw the channels in the window
 */
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
    int channelStartX = (sceneWidth - channelAreaWidth) / 2;
    int channelWidth = channelAreaWidth / currentChart.Column;

    // Draw lines for each channel
    for (int i = 0; i <= currentChart.Column; ++i) { // i <= Column ensures the outermost lines are drawn
        int x = channelStartX + i * channelWidth;

        auto* line = new QGraphicsLineItem(x, 0, x, sceneHeight);
        line->setPen(QPen(Qt::white, 4));
        line -> setZValue(3);
        scene->addItem(line);
    }
}
/*
 * load chart with wdx
 */
void PlayWindow::loadChart() {
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
        getChart(chartFile, currentChart);
        stasticChart = currentChart;
        qDebug() << "Chart loaded successfully!";

    } catch (const ChartError& e) {
        qDebug() << "Chart parsing failed:" << e.what();
    }

    fclose(chartFile);
}
/*
 * the function to play the effect when the note is clicked
 */
void PlayWindow::playColumnEffect(int column) {
    // create object
    int sceneWidth = scene->width();
    int sceneHeight = scene->height();
    int channelAreaWidth = sceneWidth * 1 / 3;
    int channelStartX = (sceneWidth - channelAreaWidth) / 2;
    int channelWidth = channelAreaWidth / currentChart.Column;
    int x = channelStartX + column * channelWidth;
    QGraphicsRectItem* effect = new QGraphicsRectItem(x, checkerLineHeight, channelWidth, 15); // 根据需要调整大小
    effect->setBrush(QBrush(Qt::green)); // set color
    scene->addItem(effect);
\
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (effect->opacity() > 0) {
            effect->setOpacity(effect->opacity() - 0.05);
        } else {
            timer->stop();
        }
    });
    timer->start(perFrame);

}
/*
 * main function to start the game
 * not the function to recv signal
 */
void PlayWindow::startGame() {// finished

    //reset the data
    initGameState();

    fileSource = ":/Chart/testChart";
    auto backgroundSource = fileSource;
    QPixmap darkenTheBackground = createDarkenedImage(backgroundSource.append(".png"), 0.4);
    if (!darkenTheBackground.isNull()) {
        auto* bgItem = new QGraphicsPixmapItem(darkenTheBackground);
        bgItem->setZValue(-1); // Ensure the background is below other items
        scene->addItem(bgItem);
        bgItem->setPixmap(darkenTheBackground.scaled(scene->width(), scene->height(), Qt::KeepAspectRatioByExpanding));
    } else {
       qDebug() << "Failed to load background image!";
    }

    // Re-add the checker line
    checkerLine = new QGraphicsLineItem(0, checkerLineHeight, scene->width(), checkerLineHeight);
    checkerLine->setPen(QPen(Qt::yellow, 4));
    scene->addItem(checkerLine);

    // Re-add the label box (stats display)
    statsLabel->setGeometry(scene->width() * 4 / 5, 8, scene->width() / 8, scene->height() / 8);
    statsLabel->setText("Accuracy: 100.00\%\nScore: 0\nCombo: 0");
    // Redraw the channels
    drawChannels();
    // Prepare the chart data
    if (!currentChart.ChartHead) {
        qDebug() << "Error: Chart data is empty. Cannot start game.";
        return;
    }


    // attention word
    QGraphicsTextItem *waiting;
    waiting = new QGraphicsTextItem("Please be prepared...");
    QFont font;
    font.setPointSize(60);
    waiting->setFont(font);
    waiting -> setPos(scene -> width() / 2 - waiting -> boundingRect().width()/2, scene -> height() / 2 - waiting->boundingRect().height() / 2);
    waiting -> setDefaultTextColor(Qt::red);
    waiting -> setZValue(100);
    scene -> addItem(waiting);

    //wait 2 seconds before starting the game
    gameStatus = 1;
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 2000) {
        QCoreApplication::processEvents();
    }

    scene -> removeItem(waiting);
    delete waiting;
    gameStatus = 0;

    // Start the game timer and the music
    musicPlayer->setPosition(0);
    musicPlayer->play();
    connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &PlayWindow::mediaStatusChanged);
    musicPlayer->setPosition(0);
    gameTimer->start(perFrame);
    qDebug() << "Game started!";
    if(musicPlayer->isPlaying())qDebug() << "music is playing";
}

void PlayWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        gameEnd();
    }
}

void PlayWindow::addFileSource(QString path)//finished
{
    fileSource = path;
}

QString PlayWindow::readFileSource()
{
    return fileSource;
}
/*
 * update the game window each frame
 * using connection of timer to trigger
 */
void PlayWindow::updateGame() { // finished
    gameTime += perFrame; // Update game time (milliseconds)
    // debug();
    spawnNotes();
    // Move notes down
    for (auto pos = notes.begin(); pos < notes.end() ;pos ++) {
        auto note = *pos;

        note.item->moveBy(0, speed); // Adjust speed as needed

        if(note.ifHold)
        {
            if(note.item->y() > checkerLineHeight)
            {
                scene->removeItem(note.item); // Remove missed notes
                notes.erase(pos);
                delete note.item ;
                currentChart.Acting -> Accuracy -= currentChart.accPerNote;
                currentChart.Acting -> maxCombo = fmax(currentChart.Acting -> maxCombo, currentChart.Acting -> Combo);
                currentChart.Acting -> Combo = 0;
                updateStats();
            }
            continue;
            auto t = note.item -> y();
            auto tt = note.length;
            auto ttt = checkerLineHeight;
            if (note.item -> y() + note.length - checkerLineHeight > MISS * speed / perFrame) {
                scene->removeItem(note.item); // Remove missed notes
                notes.erase(pos);
                delete note.item ;
                //update the accuracy logic
                currentChart.Acting -> Accuracy -= currentChart.accPerNote;
                currentChart.Acting -> maxCombo = fmax(currentChart.Acting -> maxCombo, currentChart.Acting -> Combo);
                currentChart.Acting -> Combo = 0;
                updateStats();
            }
        }
        else
        {
            if (note.item -> y() - checkerLineHeight > MISS * speed / perFrame) {
                scene->removeItem(note.item); // Remove missed notes
                notes.erase(pos);
                delete note.item ;
                // update the accuracy logic
                currentChart.Acting -> Accuracy -= currentChart.accPerNote;
                currentChart.Acting -> maxCombo = fmax(currentChart.Acting -> maxCombo, currentChart.Acting -> Combo);
                currentChart.Acting -> Combo = 0;
                updateStats();
            }
        }
    }
}
/*
 * generate the notes accoroding to the chart and the gametime
 */
void PlayWindow::spawnNotes() {// finished
    auto* measure = currentChart.ChartHead;

    double distance = scene->height() - checkerLine->y();// the distance the note need to pass
    double timeToPassDistance = distance / speed * perFrame;//the time the note to move to the checkerline
    double paintTime = gameTime - timeToPassDistance;// the time to paint the note
    double actualPaintTime = paintTime ; // pre paint it or it will be delay
    while (measure && actualPaintTime >= measure -> timeStamp) {

        for (int i = 0; i < currentChart.Column; ++i) {
            if (measure->Bar[i] > 0) { // A note exists in this column
                int channelAreaWidth = scene -> width() * 1 / 3;
                int channelWidth = channelAreaWidth / currentChart.Column;
                int channelStartX = (scene->width() - channelAreaWidth) / 2; // Center the channels
                int x = channelStartX + i * channelWidth;
                if(measure->Bar[i] == 1)// Create a falling note
                {
                    //need to change the form of the note]
                    QGraphicsRectItem* note = new QGraphicsRectItem(x, 0, channelWidth, 20);
                    note->setBrush(Qt::cyan); // Example color
                    scene->addItem(note);
                    notes.append({note, 0, 0, 0, i}); // Store in notes vector for updates
                }
                else // Create a falling note that is hold
                {
                    //need to change the form of the note]
                    int holdHeight = (measure->timeTable[i] - measure->timeStamp) * speed / perFrame ;
                    QGraphicsRectItem* note = new QGraphicsRectItem(x, 0, channelWidth, holdHeight);
                    note->setBrush(Qt::yellow); // Example color
                    scene->addItem(note);
                    notes.append({note, 1, 0, static_cast<double>(holdHeight), i}); // Store in notes vector for updates
                }
            }
        }
        // Move to the next measure
        currentChart.ChartHead = measure->NxtMea;
        measure = measure->NxtMea;
    }
}
/*
 * each time press the key to trigger the judgement
 */
void PlayWindow::keyPressEvent(QKeyEvent* event) {// cooperation with wdx
    if (event->key() == Qt::Key_Escape) {// to show the pausemenu when press esc
        if (isPaused) {
            hidePauseMenu();
        } else {
            if(gameStatus)return ;
            showPauseMenu();
        }
    }
    if (event->isAutoRepeat()){
        // qDebug() << "long press";
        return ;
    }

    if(gameStatus)//lock the process avoiding illegal operation
    {
        // qDebug() << "refused to press";
        return ;
    }
    short i = 0;
    for (; i<currentChart.Column; ++i) if (event->key() == KeyCode[i]) break;//check it if it is a valid key
    if (i == currentChart.Column) return ;//if not, return 

    playColumnEffect(i);//loading the effect

    for (auto ptr = notes.begin(); ptr < notes.end(); ++ptr){//check the notes, remember the note it lower, it is more possible to be clicked
        auto now = *ptr;
        if (now.column != i || now.holdJudge) continue;
        //distance judge, the dis is a place where you can put the macro

        if (now.ifHold) now.holdJudge = true;
        else {
            scene->removeItem(now.item);
            notes.erase(ptr);
            delete now.item;
        }
        auto dis = offsetFromLineToNote(now.item->y(), checkerLineHeight);//using the function to get the distance
        for (i = 0; MaxOffset[i] != InfOffset; ++i){// the distance judge, the maxoffset is ordered from small to larger
            if (dis > MaxOffset[i]) continue;//if the distance is larger than the maxoffset, continue .until the distance is smaller than the maxoffset, means it is in the range
            //total the result
            ++ currentChart.Acting ->judgeResult[i];
            ++ (currentChart.Acting ->Combo);
            if (!i) currentChart.Acting ->Score += 600;
            else if (i == 4) currentChart.Acting ->Score += 50;
            else currentChart.Acting ->Score += (4 - i) * 100;
            currentChart.Acting ->Accuracy -= i * 1.0l * currentChart.accPerNote / 5.0l;
            updateStats();
            break;
        }
        if (i != 5) return ;
        //total the result
        ++ currentChart.Acting ->judgeResult[i];
        currentChart.Acting ->Combo = 0;
        currentChart.Acting ->Accuracy -= 1.0l * currentChart.accPerNote;
        updateStats();
    }
    return ;
}
/*
 * special designed for the hold note
 * almost the same as below
 */
void PlayWindow::keyReleaseEvent(QKeyEvent* event) {
    if (!event->isAutoRepeat()){
        // qDebug() << "ASS";
        return ;
    }
    short i = 0;
    for (; i<currentChart.Column; ++i) if (event->key() == KeyCode[i]) break;
    if (i == currentChart.Column) return;
    for (auto ptr = notes.begin(); ptr < notes.end(); ++ptr){
        auto now = *ptr;
        if (now.ifHold && now.column == i && now.holdJudge){

            scene->removeItem(now.item);
            notes.erase(ptr);
            delete now.item;

            auto dis = offsetFromLineToNote(now.item->y(), checkerLineHeight);
            for (i = 0; MaxOffset[i] != InfOffset; ++i){
                if (dis > MaxOffset[i]) continue;
                ++ currentChart.Acting ->judgeResult[i];
                ++ currentChart.Acting ->Combo;
                if (!i) currentChart.Acting ->Score += 600;
                else if (i == 4) currentChart.Acting ->Score += 50;
                else currentChart.Acting ->Score += (4 - i) * 100;
                currentChart.Acting ->Accuracy -= i * 1.0l * currentChart.accPerNote / 5.0l;
                updateStats();
                break;
            }
            if (i != 5) return ;
            ++ currentChart.Acting ->judgeResult[i];
            currentChart.Acting ->Combo = 0;
            currentChart.Acting ->Accuracy -= 1.0l * currentChart.accPerNote;
            updateStats();
        }
    }
    return ;
}

void PlayWindow::restartGame() {

    if(gameStatus && (!isPaused))return ;
    initGameState();
    startGame();

    qDebug() << "Restarting game...";

}

void PlayWindow::continueGame() {//finishd
    if(gameStatus && (!isPaused))return ;
    hidePauseMenu();

}

void PlayWindow::exitGame() {// i finished waiting for lyjy
    qDebug() << "Exiting game...";
    if(gameStatus && (!isPaused))return ;
    initGameState();
    emit requestToHomePage();
}
/*
 * reset all!!!
 */
void PlayWindow::initGameState(){

    gameTime = 0;
    gameStatus = 0;
    scene -> clear();
    notes.clear();
    pauseMenuBackground = nullptr;
    continueButton = nullptr;
    restartButton = nullptr;
    exitButton = nullptr;
    isPaused = false;
    gameTimer->stop();
    musicPlayer->stop();
    musicPlayer->setPosition(0);
    statsLabel->setText("Accuracy: 100.00\%\nScore: 0\nCombo: 0");
    currentChart = stasticChart;
    currentChart.Acting ->Accuracy = 100.0l;
    currentChart.Acting ->Combo = 0;
    currentChart.Acting ->Score = 0;
    currentChart.Acting ->maxCombo = 0;
    memset(currentChart.Acting ->judgeResult, 0, sizeof (int) * 9);

}

Chart::ChartAct PlayWindow::gameEnd(){
    qDebug() << "Game ended!";
    currentChart.Acting -> maxCombo = fmax(currentChart.Acting -> maxCombo, currentChart.Acting -> Combo);
    //all stored in the currentChart.Acting
    //then show the result
    emit requestToResultPage();
    //storage not finished
    return *currentChart.Acting;
}
