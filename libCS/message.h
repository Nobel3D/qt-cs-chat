#ifndef MESSAGE_H
#define MESSAGE_H

#include "libcs_global.h"
#include "user.h"
#include <QXmlStreamReader>

enum Operation { Connect, Disconnect, List, Send, Recv, Join, Leave, No };

class LIBCSSHARED_EXPORT Message : public QObject
{
    Q_OBJECT
public:
    Message(QObject* parent = nullptr);
    Message(const QString &msg, QObject* parent = nullptr);
    Operation getOperation() const;
    QString getString() const;
    int getIndex() const;
    QList<User*> getUserList() const;
    void clear();

    static QString Send(const QString& text, int to);
    static QString Recv(const QString& text, int from);
    static QString Connect(const QString& name);
    static QString List(const QList<User*> ul);
    static QString Join(const QString &name, int fd);
    static QString Leave(int fd);
    static QString Disconnect(int fd);
public slots:
    Q_SLOT void newMessage(const QString& msg);
private:
    QString xmlReadNext();

    QList<User*>* ul;
    QString s;
    int i;
    Operation o;
    QXmlStreamReader* buffer;
};

#endif // MESSAGE_H
