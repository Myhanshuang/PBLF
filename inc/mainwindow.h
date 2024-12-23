#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include "../inc/Chart.h"

#include <QStackedWidget>
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    User currentUser;
    UserSaveData getUserSaveData();
private:
    QWidget *mainWidget;
    QLabel *titleLabel;
    QLineEdit *searchBox;
    QPushButton *importButton;
    QLabel *usernameLabel;
    QPushButton *settingsButton;
    QListWidget *songList;
    QLabel *previewImage;
    QStackedWidget *stackedWidget;  // 声明 QStackedWidget
    QString currentUserName;
    int baseSongIndex;

    void loadSongs();
    void updateSongList(const QString &filter = "");


private slots:
    void displaySongImage(QListWidgetItem *item);
    void searchSongs();
    void importFolder();

    void openSettings();
    void showLogoutConfirmation();
    void itemClicked(QListWidgetItem *item); // 在这里声明 itemClicked 槽函数


public slots:
    void switchToMainPage();  // 切换到主页面
    void onRequestToRestartGame();  // 重新开始游戏
    void onRequestToResultPage();
    void updateUsernameAfterLogout();
    void clearUserData();  // 新增清空用户数据的槽函数
    void handleLoginSuccess(const QString &username);

};

#ifndef KEYRHYTHM_SETTINGSWINDOW_H
#include "SettingsWindow.h"
#endif //KEYRHYTHM_CHARTWORK_H


#endif // MAINWINDOW_H
