#include "threadserver.h"


ThreadServer::ThreadServer(QTcpSocket *_s, QObject *parent) : QThread(parent)
{
    u = new User(_s->socketDescriptor());
    s = _s;
    qDebug() << u->getDescriptor() << " Connecting!";
    connect(s, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    connect(s,SIGNAL(readyRead()), this, SLOT(readMessage()), Qt::DirectConnection);
}

ThreadServer::~ThreadServer()
{
    delete u;
    delete s;
}

void ThreadServer::run()
{
    exec();
}

void ThreadServer::readMessage()
{
    QByteArray buffer;
    buffer = s->readAll();
    qDebug() << u->getDescriptor() << "[data]" << buffer;
    emit newMessage(u, new Message(buffer));
}


void ThreadServer::sendMessage(const QString &msg)
{
    if(msg.isEmpty())
        return;
    qDebug() << u->getDescriptor() << "Sending message:" << msg;
    s->write(msg.toUtf8());
    s->waitForBytesWritten();
}

void ThreadServer::disconnected()
{
    exit(0);
    s->deleteLater();

    qDebug() << u->getDescriptor() << "Disconnecting!";
    emit newMessage(u, new Message(Message::Disconnect(u->getDescriptor())));
}

User* ThreadServer::getUser() const { return u; }
