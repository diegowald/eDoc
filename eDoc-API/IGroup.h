#ifndef IGROUP_H
#define IGROUP_H

#include "forward.h"
#include "IGroupID.h"
#include <QSharedPointer>

struct IGroup
{
    virtual IGroupIDPtr groupID() = 0;
    virtual ~IGroup() {}
};


#endif // IGROUP_H
