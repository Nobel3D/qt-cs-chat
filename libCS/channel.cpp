#include "channel.h"

Channel::Channel()
{
    u = new QList<User*>;
}

void Channel::userJoin(User *user)
{
    u->append(user);
}

void Channel::userLeave()
{

}
