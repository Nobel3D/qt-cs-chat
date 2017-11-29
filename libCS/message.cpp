#include "message.h"

Message::Message(QObject *parent) : QObject(parent)
{
    buffer = new QXmlStreamReader;
    ul = new QList<User*>;
}

Message::Message(const QString& msg, QObject *parent) : QObject(parent)
{
    buffer = new QXmlStreamReader;
    ul = new QList<User*>;
    newMessage(msg);
}

void Message::clear()
{
    ul->clear();
    s.clear();
    i = -1;
    o = No;
}

QString Message::Send(const QString& text, int to) //<send>hey <b>=)</b><to>5</></>
{
    return "<send>" + text + "<to>" + QString::number(to) + "</></>";
}
QString Message::Recv(const QString& text, int from) //<recv>hey <b>=)</b><from>7</></>
{
    return "<recv>" + text + "<from>" + QString::number(from) + "</></>";
}
QString Message::Connect(const QString &name) //<connect>user</>
{
    return "<connect>" + name + "</>";
}

QString Message::Disconnect(int fd)
{
    return "<disconnect>" + QString::number(fd) + "</>";
}

QString Message::Join(const QString &name, int fd) //<join><name>user</name><fd>7</fd></>
{
    return "<join><name>" + name + "</name><fd>" + QString::number(fd) + "</fd></>";
}

QString Message::List(const QList<User *> ul) //<list><user><nick>coolname</nick><fd>5</fd></user><user><nick>othername</nick><fd>7</fd></user></>
{
    if(ul.count() == 0)
        return "";
    QString offset = "<list>";
    for(int i = 0; i < ul.count(); i++)
        offset.append("<user><nick>" + ul.at(i)->getName() + "</nick><fd>" + QString::number(ul.at(i)->getDescriptor()) + "</fd></user>");

    return offset + "</>";
}

QString Message::Leave(int fd) //<leave>5</>
{
    return "<leave>" + QString::number(fd) + "</>";
}

void Message::newMessage(const QString& msg)
{
    buffer->addData(msg);
    while(!buffer->atEnd())
    {
        if(buffer->name() == "send") //<send>hey <b>=)</b><to>5</></>
        {
            buffer->readNext();
            QString text = msg;
            text.replace("<send>","");
            text.truncate(text.indexOf("<to>")); //hey <b>=)</b>
            while(!buffer->atEnd())
            {
                buffer->readNext();
                if(buffer->name() == "to")
                {
                    buffer->readNext();
                    bool* isNumber = new bool;
                    int to = buffer->text().toInt(isNumber); //5
                    if(&isNumber)
                    {
                        s = text;
                        i = to;
                        o = Operation::Send;
                    }
                }
            }
        }
        if(buffer->name() == "recv") //<recv>hey <b>=)</b><from>7</></>
        {
            buffer->readNext();
            QString text = msg;
            text.replace("<recv>","");
            text.truncate(text.indexOf("<from>")); //hey <b>=)</b>
            while(!buffer->atEnd())
            {
                buffer->readNext();
                if(buffer->name() == "from")
                {
                    buffer->readNext();
                    bool isNumber;
                    int to = buffer->text().toInt(&isNumber); //7
                    if(isNumber)
                    {
                        s = text;
                        i = to;
                        o = Operation::Recv;
                    }
                }
            }
        }
        else if(buffer->name() == "join") //<join><name>user</name><fd>7</fd></>
        {
            buffer->readNext();
            QString nick;
            int fd;
            if (buffer->name() == "name")
            {
                nick = buffer->readElementText();
                while (buffer->name() != "fd")
                {
                    buffer->readNext();
                }
                bool isNumber;
                fd = buffer->readElementText().toInt(&isNumber);
                if(isNumber)
                {
                    s = nick;
                    i = fd;
                    o = Operation::Join;
                }
                buffer->skipCurrentElement();
            }
        }
        else if(buffer->name() == "leave")
        {
            buffer->readNext();
            bool isNumber;
            int fd = buffer->text().toInt(&isNumber);
            if(isNumber)
            {
                i = fd;
                o = Operation::Leave;
            }
        }
        else if(buffer->name() == "connect") //<connect>user</>
        {
            buffer->readNext();
            s = buffer->text().toString();
            o = Operation::Connect;
        }
        else if(buffer->name() == "disconnect") //<disconnect>5</>
        {
            buffer->readNext();
            int fd;
            bool isNumber;
            fd = buffer->text().toInt(&isNumber);
            if(isNumber)
            {
                i = fd;
                o = Operation::Disconnect;
            }
        }
        else if(buffer->name() == "list") //<list><user><nick>coolname</nick><fd>5</fd></user><user><nick>othername</nick><fd>7</fd></user></>
        {
            QString nick;
            int fd;
            while (buffer->readNextStartElement())
            {
                if (buffer->name() == "user")
                {
                    while (buffer->readNextStartElement())
                    {
                        if (buffer->name() == "nick")
                            nick = xmlReadNext();
                        else if (buffer->name() == "fd")
                        {
                            bool isNumber;
                            fd = xmlReadNext().toInt(&isNumber);
                            if(isNumber)
                            {
                                ul->append(new User(fd, nick));
                                o = Operation::List;
                            }
                        }
                        buffer->skipCurrentElement();
                    }
                }
                else
                    buffer->skipCurrentElement();
            }
        }
        buffer->readNext();
    }
    buffer->clear();
}

QString Message::xmlReadNext()
{
    buffer->readNext();
    return buffer->text().toString();
}

QList<User *> Message::getUserList() const { return *ul; }
Operation Message::getOperation() const { return o; }
QString Message::getString() const { return s; }
int Message::getIndex() const { return i; }
