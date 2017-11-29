#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    sessions = new QList<ThreadClient*>;
}

void Client::sendLogin(int fd, const QString &nick)
{
    send(fd, Message::Connect(nick));
}

void Client::deleteUser(int fd)
{
    for(int i = 0; i < ul.count(); i++)
        if(ul.at(i)->getDescriptor() == fd)
            return ul.removeAt(i);
}

User *Client::getUser(const QString &name)
{
    for(int i = 0; i < ul.count(); i++)
        if(ul.at(i)->getName() == name)
            return ul.at(i);
    return nullptr;
}

User *Client::getUser(int fd)
{
    for(int i = 0; i < ul.count(); i++)
        if(ul.at(i)->getDescriptor() == fd)
            return ul.at(i);
    return nullptr;
}

int Client::newConnection(const QString& hostname, int16_t port)
{
    sessions->append(new ThreadClient(hostname,port));
    QObject::connect(sessions->last(), SIGNAL(newMessage(Message*)), this, SLOT(newMessage(Message*)));
    sessions->last()->start();
    return sessions->last()->getDescriptor();
}

void Client::newMessage(Message *msg)
{
    switch(msg->getOperation())
    {
    case Recv:
        emit recv(msg->getIndex(), msg->getString());
        return;
    case List:
        ul = msg->getUserList();
        emit list(ul);
        return;
    case Join:
        ul.append(new User(msg->getIndex(), msg->getString()));
        emit join(msg->getIndex(), msg->getString());
        return;
    case Leave:
        deleteUser(msg->getIndex());
        emit leave(msg->getIndex());
        return;
    default:
        return;
    }
}
void Client::send(int fd, const QString &msg)
{
    for(int i = 0; i < sessions->count(); i++)
        if(sessions->at(i)->getDescriptor() == fd)
            sessions->at(i)->sendMessage(msg);
}

void Client::sendMessage(int fd, int to, const QString &msg)
{
    send(fd,Message::Send(msg, to));
}

QList<User*>* Client::getUsers() { return &ul; }
