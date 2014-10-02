#ifndef IMETADATA_H
#define IMETADATA_H

#include "forward.h"
#include <QSharedPointer>
#include "IUser.h"

class IMetadata
{
public:
    virtual IUserPtr ownerUser() = 0;
    virtual IGroupPtr ownerGroup() = 0;
    virtual bool canReadByUser(IUserPtr user) = 0;
    virtual bool canModifyByUser(IUserPtr user) = 0;
    virtual void changeUser(IUserPtr newUser) = 0;
    virtual void changeGroup(IGroupPtr newGroup) = 0;
    virtual ~IMetadata() {}
};


#endif // IMETADATA_H
