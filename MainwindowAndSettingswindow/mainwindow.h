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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *mainWidget;
    QLabel *titleLabel;
    QLineEdit *searchBox;
    QPushButton *importButton;
    QLabel *usernameLabel;
    QPushButton *settingsButton;
    QListWidget *songList;
    QLabel *previewImage;

    QString currentUserName;
    int baseSongIndex;

    void loadSongs();
    void updateSongList(const QString &filter = "");

private slots:
    void displaySongImage(QListWidgetItem *item);
    void searchSongs();
    void importFolder();
    void updateUsername(const QString &username);
    void openSettings();
    void showLogoutConfirmation();
    void itemClicked(QListWidgetItem *item); // 在这里声明 itemClicked 槽函数
};

#endif // MAINWINDOW_H
