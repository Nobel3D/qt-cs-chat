#ifndef USER_H
#define USER_H

#include "libcs_global.h"
#include <QString>

class LIBCSSHARED_EXPORT User
{
public:
    User(int _fd);
    User(int _fd, const QString& nick);
    User(int _fd, const QString& nick, int _privileges);

    void setName(const QString& _name);
    void setPrivileges(int _privileges);

    QString getName() const;
    int getPrivileges() const;
    int getDescriptor() const;
private:
    QString name;
    int fd;
    int privileges;
};

#endif // USER_H
