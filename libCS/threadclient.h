#ifndef THREADCLIENT_H
#define THREADCLIENT_H

#include "libcs_global.h"
#include "message.h"
#include <QThread>
#include <QTcpSocket>

class LIBCSSHARED_EXPORT ThreadClient : public QThread
{
    Q_OBJECT
public:
    ThreadClient(const QString& hostname, int16_t port, QObject* parent = nullptr);
    void run();
    int getDescriptor() const;
public slots:
    void sendMessage(const QString &msg);
    void readMessage();
    void disconnected();
signals:
    void error(QTcpSocket::SocketError ex);
    void newMessage(Message* msg);
private:
    QTcpSocket* c;
};

#endif // THREADCLIENT_H
