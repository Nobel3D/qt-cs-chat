#include "user.h"

User::User(int _fd)
{
    fd = _fd;
}

User::User(int _fd, const QString &nick)
{
    fd = _fd;
    name = nick;
}

User::User(int _fd, const QString &nick, int _privileges)
{
    fd = _fd;
    name = nick;
    privileges = _privileges;
}

void User::setName(const QString &_name) { name = _name; }
void User::setPrivileges(int _privileges) { privileges = _privileges; }

QString User::getName() const { return name; }
int User::getDescriptor() const { return fd; }
int User::getPrivileges() const { return privileges; }
