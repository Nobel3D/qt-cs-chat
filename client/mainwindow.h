#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <libcs.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonConnect_clicked();
    void fetch(int from, const QString& msg);
    void reloadUserTree();

private:
    Ui::MainWindow *ui;
    Client* client;
    int fd;
    int fd_selected;
};

#endif // MAINWINDOW_H
