
#include "MainWindow.h"  // 包含 MainWindow.h 头文件，以便能够识别 MainWindow 类
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include "Chart.h"
#include "SettingsWindow.h"
#include <QPalette>
#include <QBrush>
#include <QPixmap>

// 在 settingswindow.cpp 中引用 KeyCode
extern int KeyCode[9];

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
{
    // 添加返回按钮
    backButton = new QPushButton("<", this);  // 创建返回按钮
    backButton->setFixedSize(40, 40);         // 设置按钮大小
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

    // 历史记录部分
    historyList = new QListWidget(this); // 创建一个空的列表控件
    historyList->addItem("暂无历史记录"); // 初始显示一条占位信息

    // 登录部分布局
    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addWidget(new QLabel("用户名:"));
    loginLayout->addWidget(usernameInput);
    loginLayout->addWidget(new QLabel("密码:"));
    loginLayout->addWidget(passwordInput);
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(cancelButton);
    loginLayout->addWidget(registerButton);  // 将注册按钮添加到布局中
    loginLayout->setSpacing(10); // 减少控件之间的空白

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
    keyLayout->setSpacing(10); // 减少控件之间的空白


    // 左侧布局（登录 + 键位）
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(loginLayout);
    leftLayout->addLayout(keyLayout);
    leftLayout->addStretch(); // 动态填充底部空白

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout, 1);       // 左侧为登录和键位模块，权重1
    mainLayout->addWidget(historyList, 2);      // 右侧为历史记录模块，权重2
    mainLayout->setSpacing(20);                // 左右模块间距

    // 最外层布局
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(backButton, 0, Qt::AlignLeft | Qt::AlignTop); // 左上角对齐
    outerLayout->addLayout(mainLayout);     // 主布局
    outerLayout->setContentsMargins(10, 10, 10, 10); // 设置边距

    setLayout(outerLayout);






    updateBackground();

    // 设置可接受键盘事件
    setFocusPolicy(Qt::StrongFocus);

    // 信号与槽
    connect(loginButton, &QPushButton::clicked, this, &SettingsWindow::login);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::logout);
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancelLogout);
    connect(changeKeysButton, &QPushButton::clicked, this, &SettingsWindow::startKeyBinding);
    connect(backButton, &QPushButton::clicked, this, &SettingsWindow::onBackClicked);  // 返回按钮点击信号
    connect(registerButton, &QPushButton::clicked, this, &SettingsWindow::registerUser);
    connect(this, &SettingsWindow::usernameUpdated, this, &SettingsWindow::loadHistory);

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
            // 更新 currentUser
            emit loginSuccess(username);
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
        // 发射信号，通知登录成功，并更新 currentUser
        emit loginSuccess(username);  // 发射登录成功的信号
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

        // 检查当前按下的按键是否已经存在于 keysPressed 中
        if (keysPressed.contains(key)) {
            // 如果按键已经存在，给出提示信息
            QMessageBox::warning(this, "键位重复", "该键位已被绑定，请选择其他按键！");
            return;  // 忽略当前按键
        }

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


void SettingsWindow::updateBackground() {
    QPalette palette;
    QPixmap background("../../SettingsWindowBackground/SettingsWindow.jpg"); // 替换为你的图片路径
    background = background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    palette.setBrush(QPalette::Window, QBrush(background));
    setPalette(palette);
    setAutoFillBackground(true);
}

void SettingsWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event); // 保持父类的行为
    updateBackground();          // 动态更新背景图片
}

void SettingsWindow::logout() {
    // 清空历史记录
    clearHistory();

    // 清空输入框内容
    usernameInput->clear();
    passwordInput->clear();
    // 通过信号调用 MainWindow 中的槽来清空 currentUser 的数据
    MainWindow *mainWindow = dynamic_cast<MainWindow*>(parent());
    if (mainWindow) {
        mainWindow->clearUserData();  // 清空用户数据
    }


}
void SettingsWindow::loadHistory(const QString &username) {
    historyList->clear();

    QString userDir = "../../history/" + username + "/";
    QDir dir(userDir);

    if (!dir.exists()) {
        historyList->addItem("暂无历史记录");
        return;
    }

    QStringList files = dir.entryList(QDir::Files);
    for (const QString &fileName : files) {
        QFile file(userDir + fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString songTitle = in.readLine();
            QString score = in.readLine();
            QString accuracy = in.readLine();
            QString maxCombo = in.readLine();

            QString displayText = QString("%1 - 分数: %2 准确率: %3% 最大连击: %4")
                                      .arg(songTitle, score, accuracy, maxCombo);
            historyList->addItem(displayText);
            file.close();
        }
    }
}

void SettingsWindow::clearHistory() {
    historyList->clear();  // 清空历史记录列表
    historyList->addItem("暂无历史记录");  // 显示占位信息
}

void SettingsWindow::onCancelLogout() {
    // 发出取消登录的信号
    emit logoutCancelled();
}
