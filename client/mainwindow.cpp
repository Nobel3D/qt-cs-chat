#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fd = -1;
    client = new Client;
    connect(client, SIGNAL(recv(int,QString)), this, SLOT(fetch(int,QString)));
    connect(client, SIGNAL(list(QList<User*>)), this, SLOT(reloadUserTree()));
    connect(client, SIGNAL(join(int,QString)), this, SLOT(reloadUserTree()));
    connect(client, SIGNAL(leave(int)), this, SLOT(reloadUserTree()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadUserTree()
{
    QList<User*>* ul = client->getUsers();
    ui->treeWidget->clear();
    for(int i = 0; i < ul->count(); i++)
    {
        QTreeWidgetItem *buffer = new QTreeWidgetItem(ui->treeWidget);
        qDebug() << "Reload tree view:" <<ul->at(i)->getName() << ul->at(i)->getDescriptor();
        buffer->setText(0, QString::number(ul->at(i)->getDescriptor()));
        buffer->setText(1, ul->at(i)->getName());
    }
}

void MainWindow::on_pushButtonSend_clicked()
{
    if(ui->treeWidget->currentItem() == nullptr)
    {
        QMessageBox::warning(this, "Error", "Select a user!");
        return;
    }
    QString text = ui->widget->toHtml();
    int to = ui->treeWidget->currentItem()->text(0).toInt();

    if(text.isEmpty() && fd < 0 &&  to < 0)
        QMessageBox::warning(this, "Error", "Insert text or select a user!");
    else
    {
        client->sendMessage(fd, to, text);
        ui->textEdit->append("You@" + QString::number(to));
        ui->textEdit->append(text);
    }
}

void MainWindow::on_pushButtonConnect_clicked()
{
    QString hostname = ui->lineEditIp->text();
    QString nick = ui->lineEditNick->text();
    if(hostname.isEmpty() || nick.isEmpty())
        QMessageBox::warning(this,"Error", "Insert an IP address and your nickname!");

    fd = client->newConnection(hostname,27015);
    if(fd <= 0)
        QMessageBox::warning(this,"Error 404", "Server not found!");
    client->sendLogin(fd, nick);
}

void MainWindow::fetch(int from, const QString &msg)
{
    ui->textEdit->append(QString::number(from) + "@" + client->getUser(from)->getName());
    ui->textEdit->append(msg);
}

