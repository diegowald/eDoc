#ifndef IUSER_H
#define IUSER_H

#include "forward.h"
#include <QList>
#include "IGroup.h"
#include "IUserID.h"

struct IUser
{
    virtual IUserIDPtr userID() = 0;
    virtual QList<IGroupPtr> groups() = 0;
    virtual ~IUser() {}
};


#endif // IUSER_H
