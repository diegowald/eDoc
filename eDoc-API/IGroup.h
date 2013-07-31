#ifndef IGROUP_H
#define IGROUP_H

#include "IGroupID.h"

struct IGroup
{
    virtual IGroupID *groupID() = 0;
    virtual ~IGroup() {}
};

#endif // IGROUP_H
