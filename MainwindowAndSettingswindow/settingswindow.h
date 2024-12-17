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

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void login();
    void clearInputs();
    // 在 SettingsWindow 类的声明部分添加
public:
    void startKeyBinding();
    // 在 SettingsWindow 类的声明部分添加
protected:
    void keyPressEvent(QKeyEvent *event) override;


signals:
    void usernameUpdated(const QString &name);

private:
    // 登录部分
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *cancelButton;

    // 键位设置部分
    QLabel *key1Label;
    QLabel *key2Label;
    QLabel *key3Label;
    QLabel *key4Label;
    QPushButton *changeKeysButton;

    QList<int> keysPressed;  // 存储按下的键

};

#endif // SETTINGSWINDOW_H
