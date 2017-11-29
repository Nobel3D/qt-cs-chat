#include "threadclient.h"

ThreadClient::ThreadClient(const QString &hostname, int16_t port, QObject *parent) : QThread(parent)
{
    c = new QTcpSocket;
    c->connectToHost(hostname, port);
    if (!c->waitForConnected(NET_TIMEOUT))
    {
        c->setSocketDescriptor(0);
        qDebug() << c->socketDescriptor() << "Connection timeout!";
    }
    else
        qDebug() << c->socketDescriptor() << "Connected to" << hostname << ":" << port;

    connect(c,SIGNAL(readyRead()), this, SLOT(readMessage()), Qt::DirectConnection);
    connect(c, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
}

void ThreadClient::sendMessage(const QString &msg)
{
    qDebug() << getDescriptor() << "Sending message:" << msg;
    c->write(msg.toUtf8());
    c->waitForBytesWritten();
}

void ThreadClient::readMessage()
{
    QByteArray buffer;
    buffer = c->readAll();
    qDebug() << c->socketDescriptor() << "[data]" << buffer;
    emit newMessage(new Message(buffer));
}

void ThreadClient::run()
{
    exec();
}
void ThreadClient::disconnected()
{
    exit(0);
    c->deleteLater();

    qDebug() << "Disconnecting!";
}

int ThreadClient::getDescriptor() const { return c->socketDescriptor(); }
