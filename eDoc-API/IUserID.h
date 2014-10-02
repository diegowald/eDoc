#ifndef IUSERID_H
#define IUSERID_H

#include "forward.h"

class IUserID
{
public:
    virtual QString asString() const = 0;
    virtual ~IUserID() {}
};


#endif // IUSERID_H
