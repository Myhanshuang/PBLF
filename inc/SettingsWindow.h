#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextStream>
#include <QFile>
#include <QListWidget>
#include "../inc/User.h"  // 添加这一行来包含 User 类的头文件


class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();



private slots:
    void login();

    void onBackClicked();  // 添加 onBackClicked 槽函数声明
    void registerUser();
    void onCancelLogout();
public:
    void startKeyBinding();
    // 在 SettingsWindow 类的声明部分添加
    void loadHistory(const QString &username);
    void logout();
    void clearHistory();
    void loadUserSaveData();
protected:
    void keyPressEvent(QKeyEvent *event) override;



signals:
    void usernameUpdated(const QString &name);
    void loginSuccess(const QString &username);
    void backToMainPage();  // 添加返回主页面的信号
    void logoutCancelled();
    void onRequestToLoadHistory();

private:
    QPushButton *backButton;  // 声明返回按钮
    QPushButton *registerButton;  // 新增的注册按钮

    // 登录部分
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    int keyStatus = 0;
    // 键位设置部分
    QLabel *key1Label;
    QLabel *key2Label;
    QLabel *key3Label;
    QLabel *key4Label;
    QPushButton *changeKeysButton;
    QListWidget *historyList;  // 声明历史记录列表变量

    QList<int> keysPressed;  // 存储按下的键
    void updateBackground();  // 声明 updateBackground 方法
   void onHistoryItemClicked(QListWidgetItem *item);

};

#endif // SETTINGSWINDOW_H
