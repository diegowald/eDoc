#ifndef IUSER_H
#define IUSER_H

#include <QList>
#include "IGroup.h"
#include "IUserID.h"
struct IUser
{
    virtual IUserID *userID() = 0;
    virtual QList<IGroup*> groups() = 0;
    virtual ~IUser() {}
};

#endif // IUSER_H
