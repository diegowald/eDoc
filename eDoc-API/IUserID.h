#ifndef IUSERID_H
#define IUSERID_H

#include <QSharedPointer>

struct IUserID
{
public:
    virtual QString asString() const = 0;
    virtual ~IUserID() {}
};

typedef QSharedPointer<IUserID> IUserIDPtr;

#endif // IUSERID_H
