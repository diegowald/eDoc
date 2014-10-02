#ifndef IGROUPID_H
#define IGROUPID_H

#include "forward.h"
#include <QSharedPointer>

class IGroupID
{
public:
    virtual QString asString() const = 0;
    virtual ~IGroupID() {}
};



#endif // IGROUPID_H
