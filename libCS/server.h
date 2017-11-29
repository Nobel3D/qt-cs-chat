#ifndef SERVER_H
#define SERVER_H

#include "libcs_global.h"
#include "channel.h"
#include "user.h"
#include "threadserver.h"
#include "message.h"
#include <QObject>
#include <QTcpServer>

class LIBCSSHARED_EXPORT Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(int max_connections = 10, QTcpServer* parent = nullptr);
    void startServer(int16_t port = 27015);
public slots:
    void newConnection();
    void newMessage(User* u, Message* msg);
    void send(User *handle, int fd, const QString &msg);
    void sendAll(const QString &msg);
private:
    void disconnected(int fd);
    void sendMessage(User* handle, int to, const QString& text);
    void joinMessage(User* handle, const QString& where);
    void leaveMessage(User* handle, const QString& where);
    void listMessage(User* handle, const QString& name);
    QList<User*> getUserList(const User *except);
    int16_t p;
    Channel* c;
    QList<ThreadServer*>* sessions;
};

#endif // SERVER_H
