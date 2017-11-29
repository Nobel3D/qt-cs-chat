#ifndef THREADSERVER_H
#define THREADSERVER_H

#include "libcs_global.h"
#include "user.h"
#include "message.h"
#include <QThread>
#include <QTcpSocket>
#include <QDebug>


class LIBCSSHARED_EXPORT ThreadServer : public QThread
{
    Q_OBJECT
public:
    explicit ThreadServer(QTcpSocket* _s, QObject *parent = 0);
    ~ThreadServer();
    void run();
    User* getUser() const;
signals:
    void error(QTcpSocket::SocketError ex);
    void newMessage(User* u, Message* msg);
    void disconnected(ThreadServer* session);

public slots:
    void disconnected();
    void sendMessage(const QString& msg);
    void readMessage();

private:
    User* u;
    QTcpSocket* s;

};

#endif // THREADSERVER_H
