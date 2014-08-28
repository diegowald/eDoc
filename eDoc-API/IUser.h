#ifndef IUSER_H
#define IUSER_H

#include <QList>
#include "IGroup.h"
#include "IUserID.h"

struct IUser
{
    virtual QSharedPointer<IUserID> userID() = 0;
    virtual QList<QSharedPointer<IGroup> > groups() = 0;
    virtual ~IUser() {}
};

#endif // IUSER_H
