#ifndef CHANNEL_H
#define CHANNEL_H

#include "libcs_global.h"
#include <user.h>
#include <QList>


class LIBCSSHARED_EXPORT Channel
{
public:
    Channel();
    void userJoin(User* user);
    void userLeave();
private:
    QList<User*>* u;
};

#endif // CHANNEL_H
