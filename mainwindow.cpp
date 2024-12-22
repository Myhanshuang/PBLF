#include "mainwindow.h"
#include "SettingsWindow.h"  // 添加这一行
#include <QStackedWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication> // 用于获取当前应用程序路径
#include <QPixmapCache>     // 用于缓存图片
#include "playwindow.h"  // 导入 PlayWindow
#include "ResultPage.h"  // 包含 ResultPage 头文件

ResultPage *resultPage;
PlayWindow *playWindow;
SettingsWindow *settingsWindow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentUserName("未登录"),
    baseSongIndex(1) {

    // 创建 QStackedWidget 控件
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // 创建 MainWindow 页面，直接将页面布局添加到 QStackedWidget
    QWidget *mainWidget = new QWidget(this);
    stackedWidget->addWidget(mainWidget);



    // 初始化显示主页面
    stackedWidget->setCurrentIndex(0);  // 显示 MainWindow 页面

    // 顶部布局
    titleLabel = new QLabel("音游", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("搜索曲目...");
    QPushButton *searchButton = new QPushButton("搜索", this);

    importButton = new QPushButton("导入", this);
    usernameLabel = new QLabel(currentUserName, this);

    usernameLabel->setText("未登录");


    settingsButton = new QPushButton("⚙️", this);
    settingsButton->setFixedSize(30, 30);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(searchBox);
    topLayout->addWidget(searchButton);
    topLayout->addStretch();
    topLayout->addWidget(importButton);
    topLayout->addWidget(usernameLabel);
    topLayout->addWidget(settingsButton);

    // 主体布局
    songList = new QListWidget(this);
    previewImage = new QLabel(this);
    previewImage->setAlignment(Qt::AlignCenter);
    previewImage->setFixedSize(800, 720);//修改主页图片大小！！！！

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(songList, 1);
    mainLayout->addWidget(previewImage, 2);

    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->addLayout(topLayout);
    layout->addLayout(mainLayout);

    setStyleSheet(R"(
    QMainWindow {
        background-color: white;  /* 设置主窗口背景为白色 */
    }

QWidget {
        background-color: white;  /* 所有 QWidget 派生类背景为白色 */
    }

    QLineEdit, QPushButton, QListWidget, QLabel {
        background-color: white;  /* 设置控件背景为白色 */
        color: black;  /* 字体颜色为黑色 */
        border: 2px solid black; /* 黑色边框 */
        border-radius: 5px; /* 圆角效果 */
        padding: 5px; /* 内边距 */
        font-size: 18px; /* 字体大小 */
    }

    QPushButton:hover {
        background-color: rgba(200, 200, 200, 255); /* 鼠标悬浮时更亮 */
    }

    QListWidget::item {
        color: black; /* 列表项字体颜色 */
    }

    QListWidget {
        background-color: white; /* 设置列表背景为白色 */
        border: 2px solid black; /* 列表项边框颜色 */
    }

    QLabel#titleLabel {
        font-size: 28px; /* 标题字体更大 */
        font-weight: bold;
        color: black; /* 标题文字颜色为黑色 */
    }
)");



    // 加载歌曲列表
    loadSongs();

    // 信号与槽
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchSongs);
    connect(songList, &QListWidget::itemEntered, this, &MainWindow::displaySongImage);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(importButton, &QPushButton::clicked, this, &MainWindow::importFolder);
    connect(usernameLabel, &QLabel::linkActivated, this, &MainWindow::showLogoutConfirmation);


    setWindowTitle("音游主界面");
    resize(1280, 720);

    // 创建 SettingsWindow 页面
    settingsWindow = new SettingsWindow(this);
    stackedWidget->addWidget(settingsWindow);

    connect(settingsWindow, &SettingsWindow::loginSuccess, this, &MainWindow::switchToMainPage);
    connect(settingsWindow, &SettingsWindow::usernameUpdated, this, &MainWindow::updateUsername);
    connect(settingsWindow, &SettingsWindow::backToMainPage, this, &MainWindow::switchToMainPage);  // 连接返回主页面的信号
    connect(settingsWindow, &SettingsWindow::logoutCancelled, this, &MainWindow::updateUsernameAfterLogout);
    // 创建 PlayWindow 页面
    // PlayWindow *
    playWindow = new PlayWindow(this);
    stackedWidget->addWidget(playWindow);

    connect(playWindow, &PlayWindow::requestToHomePage, this, &MainWindow:: switchToMainPage);


    // 创建 ResultPage 页面
    // ResultPage
    resultPage = new ResultPage(this);
    stackedWidget->addWidget(resultPage);

    // 信号与槽连接
    connect(resultPage, &ResultPage::requestToHomePage, this, &MainWindow::switchToMainPage);
    connect(resultPage, &ResultPage::requestToRestartGame, this, &MainWindow::onRequestToRestartGame);

}

MainWindow::~MainWindow() {}


void MainWindow::loadSongs() {
    songList->clear();

    // 临时断开信号连接，避免加载过程中触发点击事件
    disconnect(songList, &QListWidget::itemClicked, this, &MainWindow::itemClicked);

    // 构造正确的相对路径
    QString baseDir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../../..");
    QString chartPath = baseDir + "/KeyRhythm/chart";
    QDir chartDir(chartPath);

    if (!chartDir.exists()) {
        qDebug() << "目录不存在:" << chartPath; // 调试输出
        return;
    }

    // 获取所有子目录
    QStringList subDirs = chartDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &subDir : subDirs) {
        QString songDir = chartDir.absoluteFilePath(subDir);
        QDir dir(songDir);

        // 搜索图片文件
        QStringList images = dir.entryList(QStringList() << "*.jpg", QDir::Files);
        if (!images.isEmpty()) {
            QString imagePath = dir.absoluteFilePath(images.first());

            // 添加到列表项
            QListWidgetItem *item = new QListWidgetItem(subDir, songList);
            item->setData(Qt::UserRole, imagePath);


            // 预缓存图片
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
                QPixmapCache::insert(imagePath, pixmap);
            } else {
                qDebug() << "无法加载图片:" << imagePath; // 调试输出
            }
        }
    }

    // 重新连接信号
    connect(songList, &QListWidget::itemClicked, this, &MainWindow::itemClicked);

    // 启用悬停信号
    connect(songList, &QListWidget::itemEntered, this, &MainWindow::displaySongImage);
    connect(songList, &QListWidget::itemClicked, this, &MainWindow::itemClicked);
    songList->setMouseTracking(true);
    songList->setAttribute(Qt::WA_Hover);
}

void MainWindow::displaySongImage(QListWidgetItem *item) {
    QString imagePath = item->data(Qt::UserRole).toString();
    if (!imagePath.isEmpty()) {
        QPixmap pixmap;

        // 从缓存中加载图片
        if (!QPixmapCache::find(imagePath, &pixmap)) {
            pixmap.load(imagePath); // 如果缓存中没有，则重新加载
            QPixmapCache::insert(imagePath, pixmap);
        }

        if (!pixmap.isNull()) {
            // 设置图片
            previewImage->setPixmap(pixmap.scaled(previewImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qDebug() << "图片加载失败:" << imagePath; // 调试输出
            previewImage->clear();
            previewImage->setText("无图片");
        }
    } else {
        previewImage->clear();
        previewImage->setText("无图片");
    }
}

void MainWindow::searchSongs() {
    QString filter = searchBox->text();
    updateSongList(filter);
}

void MainWindow::updateSongList(const QString &filter) {
    songList->clear();
    QDir chartDir("../../../KeyRhythm/chart");
    if (!chartDir.exists()) return;

    QStringList subDirs = chartDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &subDir : subDirs) {
        if (!filter.isEmpty() && !subDir.contains(filter, Qt::CaseInsensitive)) continue;

        QString songDir = chartDir.absoluteFilePath(subDir);
        QDir dir(songDir);
        QStringList images = dir.entryList(QStringList() << "*.jpg", QDir::Files);

        QListWidgetItem *item = new QListWidgetItem(subDir, songList);
        if (!images.isEmpty()) {
            item->setData(Qt::UserRole, dir.absoluteFilePath(images.first()));
        }
    }
}
//导入功能
void MainWindow::importFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "选择歌曲文件夹");
    if (folderPath.isEmpty()) return;

    QDir folder(folderPath);
    //这里可以修改可以传入的文件格式
    //检查所选文件夹是否包含必需的文件类型（.mc, .jpg, .ogg），并且至少有三个文件。如果没有，则弹出警告消息并返回。
    QStringList requiredFiles = folder.entryList(QStringList() << "*.mc" << "*.jpg" << "*.ogg", QDir::Files);
    if (requiredFiles.size() < 3) {
        QMessageBox::warning(this, "错误", "文件夹中缺少必要文件！");
        return;
    }

    QString newFolder = QString("../../../KeyRhythm/chart/base%1").arg(baseSongIndex++);
    QDir().mkdir(newFolder);

    for (const QString &file : requiredFiles) {
        QFile::copy(folder.absoluteFilePath(file), newFolder + "/" + file);
    }

    QMessageBox::information(this, "导入成功", "歌曲已导入！");
    loadSongs();
}

void MainWindow::updateUsername(const QString &name) {
    currentUserName = name;  // 更新用户名
    if (currentUserName == "未登录") {
        // 未登录时禁用点击事件
        usernameLabel->setText(currentUserName);
        usernameLabel->setTextInteractionFlags(Qt::NoTextInteraction);  // 禁用文本交互
    } else {
        // 已登录时设置为可点击链接
        usernameLabel->setText("<a href='#'>" + currentUserName + "</a>");
        usernameLabel->setTextInteractionFlags(Qt::TextBrowserInteraction); // 启用文本交互
    }
}


void MainWindow::openSettings()
{
    stackedWidget->setCurrentIndex(1);
}





void MainWindow::showLogoutConfirmation()
{
    if (currentUserName != "未登录") {
        // 用户已登录，弹出退出登录确认框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "退出登录", "您确定要退出登录吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        // 设置对话框背景为白色
        QMessageBox *msgBox = qobject_cast<QMessageBox*>(this->sender());
        if (msgBox) {
            msgBox->setStyleSheet("QMessageBox { background-color: white; }");
        }
        if (reply == QMessageBox::Yes) {
            currentUserName = "未登录";  // 清空当前用户名
            usernameLabel->setText(currentUserName); // 更新显示的用户名
            settingsWindow->logout();

        }
    }
}




void MainWindow::itemClicked(QListWidgetItem *item) {
    // 防止多次弹出对话框，可以使用一个状态标志来标记是否已经弹出过对话框
    static bool isDialogOpen = false;
    if (isDialogOpen) return;  // 如果对话框已经弹出，直接返回

    isDialogOpen = true;  // 标记对话框已弹出

    QString songFolderName = item->text();  // 获取歌曲文件夹名
    QString folderPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../../..") + "/KeyRhythm/chart/" + songFolderName;

    // 弹出确认框询问是否进入游戏
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "进入游戏", "是否进入游戏？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // 构造传递给游戏窗口的路径
        QString fileSource = ":/Chart/" + songFolderName + "/" + songFolderName;
        /* qDebug() << "fileSource: " << fileSource;
        if (!QFile::exists(fileSource)) {
            qDebug() << "File does not exist!";
            return;//测试用
        }*/

        // 获取 PlayWindow 并传递文件路径
        PlayWindow *playWindow = qobject_cast<PlayWindow*>(stackedWidget->widget(2));
        if (playWindow) {
            playWindow->addFileSource(fileSource);  // 将文件路径传递给 PlayWindow
            stackedWidget->setCurrentIndex(2);  // 切换到游戏页面
            playWindow->start();
        }
    }
    // 重置标志，允许下一次点击
    isDialogOpen = false;
}
// 切换到主页面
void MainWindow::switchToMainPage() {
    stackedWidget->setCurrentIndex(0);  // 切换到 MainWindow 页面
}



void MainWindow::onRequestToRestartGame() {
    stackedWidget->setCurrentIndex(2);
    playWindow->restartGame();
}

void MainWindow::onRequestToResultPage() {
    //上面这个函数能正常运作不re就对
    //这个函数等wdx更新了就对了
    char evaluation = 'F';
    if(playWindow->currentChart.Acting->Accuracy >= 95){
        evaluation = 'S';
    } else if(playWindow->currentChart.Acting->Accuracy >= 90){
        evaluation = 'A';
    } else if(playWindow->currentChart.Acting->Accuracy >= 80){
        evaluation = 'B';
    } else if(playWindow->currentChart.Acting->Accuracy >= 70){
        evaluation = 'C';
    }

    //    resultPage->setResults(playWindow->currentChart.Acting->judgeResult[0],
    //                       playWindow->currentChart.Acting->judgeResult[1],
    //                       playWindow->currentChart.Acting->judgeResult[2],
    //                       playWindow->currentChart.Acting->judgeResult[3],
    //                       playWindow->currentChart.noteCount - playWindow->currentChart.Acting->judgeResult[0] - playWindow->currentChart.Acting->judgeResult[1] - playWindow->currentChart.Acting->judgeResult[2] - playWindow->currentChart.Acting->judgeResult[3],
    //                       playWindow->currentChart.Acting->Accuracy,
    //                       playWindow->currentChart.Acting->maxCombo,
    //                       evaluation,
    //                       playWindow->currentChart.Acting->Score,
    //                       playWindow->currentChart.songTitle,
    //                       playWindow->readFileSource().append(".png"));
    stackedWidget->setCurrentIndex(3);  // 切换到结果页面
    qDebug() << "Navigating to Result Page...";
}


void MainWindow::updateUsernameAfterLogout() {
    currentUserName = "未登录";  // 更新用户名为“未登录”
    usernameLabel->setText(currentUserName);  // 更新显示的用户名
}
