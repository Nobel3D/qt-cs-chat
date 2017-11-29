#include "server.h"

Server::Server(int max_connections, QTcpServer* parent) : QTcpServer(parent)
{
    sessions = new QList<ThreadServer*>;
    setMaxPendingConnections(max_connections);
    QObject::connect(this,SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::startServer(int16_t port)
{
    if(!listen(QHostAddress::Any, port))
        qDebug() << "Impossible start socket at " << QString::number(port);
    else
        qDebug() << "Server is listening at " << QString::number(port);

}

void Server::send(User* handle, int fd, const QString &msg)
{
    for(int i = 0; i < sessions->count(); i++)
        if(sessions->at(i)->getUser()->getDescriptor() == fd)
            sessions->at(i)->sendMessage(msg);
}

void Server::sendAll(const QString &msg)
{
    for(int i = 0; i < sessions->count(); i++)
        sessions->at(i)->sendMessage(msg);
}

void Server::sendMessage(User *handle, int to, const QString &text)
{
    send(handle, to, Message::Recv(text,to));
}

void Server::listMessage(User *handle, const QString &name)
{
    handle->setName(name);
    qDebug() << "Connected user" << handle->getName() << "ID:" << handle->getDescriptor();
    send(handle, handle->getDescriptor(), Message::List(getUserList(handle)));
}

void Server::newConnection()
{
    ThreadServer* buffer = new ThreadServer(nextPendingConnection(), this);
    sessions->append(buffer);
    QObject::connect(buffer, SIGNAL(newMessage(User*,Message*)), this, SLOT(newMessage(User*,Message*)), Qt::AutoConnection);
    sessions->last()->start();
}

void Server::disconnected(int fd)
{
    sendAll(Message::Leave(fd));
    for(int i = 0; i < sessions->count(); i++)
        if(sessions->at(i)->getUser()->getDescriptor() == fd)
            sessions->removeAt(i);
}

void Server::newMessage(User *u, Message *msg)
{
    switch(msg->getOperation())
    {
    case Connect:
        listMessage(u, msg->getString());
        sendAll(Message::Join(u->getName(), u->getDescriptor()));
        return;
    case Send:
        sendMessage(u, msg->getIndex(), msg->getString());
        return;
    case Disconnect:
        disconnected(msg->getIndex());
    default:
        return;
    }
}

QList<User*> Server::getUserList(const User* except)
{
    QList<User*> offset;
    for(int i = 0; i < sessions->count(); i++)
        if(except->getDescriptor() != sessions->at(i)->getUser()->getDescriptor())
            offset.append(sessions->at(i)->getUser());

    return offset;
}
