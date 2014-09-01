#ifndef IGROUP_H
#define IGROUP_H

#include "IGroupID.h"
#include <QSharedPointer>

struct IGroup
{
    virtual IGroupIDPtr groupID() = 0;
    virtual ~IGroup() {}
};

typedef QSharedPointer<IGroup> IGroupPtr;
#endif // IGROUP_H
