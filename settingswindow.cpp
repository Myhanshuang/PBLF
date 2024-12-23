
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


    setStyleSheet(R"(
    QLineEdit, QPushButton, QListWidget, QLabel {
         background-color: rgba(240, 240, 240, 255); /* 控件背景为浅灰色，区分主背景 */
        color: black; /* 字体颜色为黑色 */
        border: 1px solid rgba(200, 200, 200, 255); /* 边框颜色为浅灰色 */
        border-radius: 5px; /* 圆角效果 */
        padding: 5px; /* 内边距 */
        font-size: 18px; /* 字体大小 */
    }

    QPushButton:hover {
        background-color: rgba(230, 230, 230, 255); /* 鼠标悬浮时更亮 */
    }

    QListWidget::item {
        color: black; /* 列表项字体颜色 */
    }

    QListWidget {
        background-color: rgba(255, 255, 255, 200); /* 列表背景半透明 */
        border: 1px solid rgba(200, 200, 200, 255); /* 边框颜色为浅灰色 */
        border-radius: 5px; /* 圆角效果 */
    }

    QLabel#titleLabel {
        font-size: 28px; /* 标题字体更大 */
        font-weight: bold;
        color: black; /* 标题文字颜色为黑色 */
        background: none; /* 无背景 */
    }
)");





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
    connect(historyList, &QListWidget::itemClicked, this, &SettingsWindow::onHistoryItemClicked);

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
    // 设置纯白背景
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);
    setAutoFillBackground(true);
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
            QString timestampStr = in.readLine(); // 第五行（时间戳）

            // 将第五行内容（时间戳）转换为 time_t 类型
            bool ok;
            time_t timestamp = timestampStr.toLongLong(&ok);
            QString formattedTime;

            if (ok) {
                // 将时间戳转换为 QDateTime
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
                formattedTime = dateTime.toString("yyyy-MM-dd HH:mm:ss"); // 设置时间显示格式
            } else {
                formattedTime = "无效时间戳"; // 如果转换失败，显示无效时间戳
            }

            // 构建显示文本
            QString displayText = QString("%1 - 分数: %2 准确率: %3% 最大连击: %4 时间: %5")
                                      .arg(songTitle, score, accuracy, maxCombo, formattedTime);
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

void SettingsWindow::onHistoryItemClicked(QListWidgetItem *item){
    // 弹出提示框，确认是否删除
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "删除历史记录",
        QString("您是否要删除以下历史记录？\n\n%1").arg(item->text()),
        QMessageBox::Yes | QMessageBox::No
        );

    // 打印完整历史记录文本，查看其结构
    qDebug() << "History record text:" << item->text();

    // 按空格分割字符串，获取时间部分
    QStringList parts = item->text().split(" ");  // 按空格分割
    qDebug() << "Parts after split:" << parts;
    if (parts.size() >= 16) {  // 确保有时间部分
        // 去掉"时间:"，然后拼接日期和时间
        QString dateString = parts[15] + " " + parts[16];  // parts[15] 是日期，parts[16] 是时间
        qDebug() << "Extracted time string:" << dateString;

        // 将字符串转换为 QDateTime 对象
        QDateTime dateTime = QDateTime::fromString(dateString, "yyyy-MM-dd hh:mm:ss");
        if (!dateTime.isValid()) {
            qDebug() << "Error: Invalid time string extracted.";
            return;
        }

        // 转换为 Unix 时间戳
        time_t timeStamp = dateTime.toSecsSinceEpoch();
        qDebug() << "Converted timestamp:" << timeStamp;



        // 如果用户选择“是”，则执行删除逻辑
        if (reply == QMessageBox::Yes) {
            QWidget *parentWidget = qobject_cast<QWidget*>(parent());

            while (parentWidget && !qobject_cast<MainWindow*>(parentWidget)) {
                parentWidget = parentWidget->parentWidget();
            }

            if (parentWidget) {
                MainWindow *mainWindow = qobject_cast<MainWindow*>(parentWidget);
                if (mainWindow) {
                    qDebug() << "Successfully cast parent to MainWindow.";

                    // 获取 UserSaveData 对象
                    UserSaveData userSaveData = mainWindow->getUserSaveData();

                  /*  // 获取历史记录文件的完整路径（假设文件名为时间戳）
                    QString historyFilePath = "../../history/" + QString::number(timeStamp);
                    qDebug() << "History file path: " << historyFilePath;*/

                    // 调用 removeData 删除历史记录
                    try {
                        if (userSaveData.removeData("../../history", timeStamp)) {
                            QMessageBox::information(this, "删除成功", "历史记录已删除！");
                            loadHistory(mainWindow->currentUser.userName);  // 更新历史记录列表
                        } else {
                            QMessageBox::warning(this, "删除失败", "删除历史记录失败！");
                        }
                    } catch (const ChartError &e) {
                        qDebug() << "ChartError occurred: " << e.what();
                        QMessageBox::warning(this, "删除失败", "删除历史记录时发生错误！");
                    }
                }
            } else {
                qDebug() << "Failed to find MainWindow in parent hierarchy.";
            }
        }
    } else {
        qDebug() << "Error: Not enough parts in the history record text.";
    }
}





