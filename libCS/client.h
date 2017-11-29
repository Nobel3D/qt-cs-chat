#ifndef CLIENT_H
#define CLIENT_H

#include "libcs_global.h"
#include "user.h"
#include "threadclient.h"
#include <QList>

class LIBCSSHARED_EXPORT Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);
    void sendLogin(int fd, const QString& nick);
    void sendMessage(int fd, int to, const QString& msg);

    void deleteUser(int fd);
    User *getUser(const QString &name);
    User *getUser(int fd);

    QList<User *> *getUsers();
public slots:
    int newConnection(const QString &hostname, int16_t port);
    void newMessage(Message *msg);
    void send(int fd, const QString& msg);
signals:
    void recv(int from, const QString& msg);
    void list(const QList<User*>& map);
    void join(int fd, const QString &name);
    void leave(int fd);
private:
    User* me; //TODO: find client user into "ul"
    QList<User*> ul;
    QList<ThreadClient*>* sessions;
};

#endif // CLIENT_H
