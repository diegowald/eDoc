#ifndef IUSER_H
#define IUSER_H

#include <QList>
#include "IGroup.h"
#include "IUserID.h"

struct IUser
{
    virtual IUserIDPtr userID() = 0;
    virtual QList<IGroupPtr> groups() = 0;
    virtual ~IUser() {}
};

typedef QSharedPointer<IUser> IUserPtr;
#endif // IUSER_H
