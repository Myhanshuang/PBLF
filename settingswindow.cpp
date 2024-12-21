#include "SettingsWindow.h"
#include "MainWindow.h"  // 包含 MainWindow.h 头文件，以便能够识别 MainWindow 类
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include "Chart.h"
#include "User.h"  // 引入 User 类

// 在 settingswindow.cpp 中引用 KeyCode
extern int KeyCode[9];

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    // 添加返回按钮
    backButton = new QPushButton("<", this);  // 创建返回按钮
    backButton->setFixedSize(30, 30);         // 设置按钮大小
    backButton->setStyleSheet("font-size: 20px; font-weight: bold;");  // 设置按钮样式，便于显示 "<"


    // 登录部分
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("请输入用户名");
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("请输入密码");
    loginButton = new QPushButton("登录", this);
    cancelButton = new QPushButton("取消", this);
    registerButton = new QPushButton("注册", this);  // 新增注册按钮


    // 键位设置部分
    key1Label = new QLabel("未设置", this);
    key2Label = new QLabel("未设置", this);
    key3Label = new QLabel("未设置", this);
    key4Label = new QLabel("未设置", this);
    changeKeysButton = new QPushButton("开始修改键位", this);

    // 登录部分布局
    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addWidget(new QLabel("用户名:"));
    loginLayout->addWidget(usernameInput);
    loginLayout->addWidget(new QLabel("密码:"));
    loginLayout->addWidget(passwordInput);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(cancelButton);
    loginLayout->addWidget(registerButton);  // 将注册按钮添加到布局中

    // 键位设置部分布局
    QVBoxLayout *keyLayout = new QVBoxLayout;
    keyLayout->addWidget(new QLabel("键位1:"));
    keyLayout->addWidget(key1Label);
    keyLayout->addWidget(new QLabel("键位2:"));
    keyLayout->addWidget(key2Label);
    keyLayout->addWidget(new QLabel("键位3:"));
    keyLayout->addWidget(key3Label);
    keyLayout->addWidget(new QLabel("键位4:"));
    keyLayout->addWidget(key4Label);
    keyLayout->addWidget(changeKeysButton);

    // 总布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(backButton);  // 将返回按钮添加到布局
    mainLayout->addLayout(loginLayout);
    mainLayout->addLayout(keyLayout);

    setLayout(mainLayout);

    // 信号与槽
    connect(loginButton, &QPushButton::clicked, this, &SettingsWindow::login);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::clearInputs);
    connect(changeKeysButton, &QPushButton::clicked, this, &SettingsWindow::startKeyBinding);
    connect(backButton, &QPushButton::clicked, this, &SettingsWindow::onBackClicked);  // 返回按钮点击信号
    connect(registerButton, &QPushButton::clicked, this, &SettingsWindow::registerUser);

    // 设置可接受键盘事件
    setFocusPolicy(Qt::StrongFocus);
}

SettingsWindow::~SettingsWindow() {}
// 登录功能
void SettingsWindow::login()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // 创建 User 对象
    User user;
    strncpy(user.userName, username.toStdString().c_str(), sizeof(user.userName));
    strncpy(user.Password, password.toStdString().c_str(), sizeof(user.Password));

    // 打开文件进行验证
    FILE *file = fopen("../../user_credentials.txt", "r");  // 打开文件用于验证用户
    if (file == nullptr) {
        QMessageBox::warning(this, "文件错误", "无法打开用户数据文件！");
        return;
    }

    // 检查用户名是否已存在
    if (user.newUser(file)) {
        QMessageBox::warning(this, "登录失败", "该用户尚未注册！");
    } else {
        // 检查密码是否正确
        if (user.correct(file)) {
            QMessageBox::information(this, "登录成功", "登录成功！");
            emit loginSuccess();
            emit usernameUpdated(username);
            close();  // 登录成功后关闭设置窗口
        } else {
            QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
        }
    }
    fclose(file);  // 关闭文件
}
// 注册功能
void SettingsWindow::registerUser()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名或密码不能为空！");
        return;
    }

    // 创建 User 对象
    User user;
    strncpy(user.userName, username.toStdString().c_str(), sizeof(user.userName));
    strncpy(user.Password, password.toStdString().c_str(), sizeof(user.Password));

    // 打开文件进行保存
    FILE *file = fopen("../../user_credentials.txt", "a");  // 打开文件用于保存新用户
    if (file == nullptr) {
        QMessageBox::warning(this, "文件错误", "无法打开用户数据文件！");
        return;
    }

    try {
        user.save(file);  // 保存用户数据
        QMessageBox::information(this, "注册成功", "用户注册成功！");
        emit usernameUpdated(username);

    } catch (ChartError &e) {
        QMessageBox::warning(this, "注册失败", "保存用户信息失败！");
    }
    fclose(file);  // 关闭文件
}

void SettingsWindow::onBackClicked()
{
    emit backToMainPage();  // 发射信号，返回主页面
}

// 开始修改键位
void SettingsWindow::startKeyBinding() {
    // 开始键位绑定，清空已按下的键
    keysPressed.clear();
    key1Label->setText("未设置");
    key2Label->setText("未设置");
    key3Label->setText("未设置");
    key4Label->setText("未设置");
    QMessageBox::information(this, "键位绑定", "开始按下四个键位！");
}

void SettingsWindow::keyPressEvent(QKeyEvent *event) {
    if (keysPressed.size() < 4) {  // 限制最多只记录四个按键
        int key = event->key();
        keysPressed.append(key);  // 保存按下的键

        // 更新界面上的标签
        if (keysPressed.size() == 1) {
            key1Label->setText(QKeySequence(key).toString());
        } else if (keysPressed.size() == 2) {
            key2Label->setText(QKeySequence(key).toString());
        } else if (keysPressed.size() == 3) {
            key3Label->setText(QKeySequence(key).toString());
        } else if (keysPressed.size() == 4) {
            key4Label->setText(QKeySequence(key).toString());
            // 将按下的键码保存到 KeyCode 数组
            for (int i = 0; i < keysPressed.size(); ++i) {
                KeyCode[i] = keysPressed[i];  // 更新 KeyCode 数组
            }
            QMessageBox::information(this, "键位绑定", "四个键位已绑定！");
        }
    }
}





void SettingsWindow::clearInputs()
{
    usernameInput->clear();
    passwordInput->clear();
}
