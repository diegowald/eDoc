#ifndef IGROUPID_H
#define IGROUPID_H

#include <QSharedPointer>

struct IGroupID
{
public:
    virtual QString asString() const = 0;
    virtual ~IGroupID() {}
};

typedef QSharedPointer<IGroupID> IGroupIDPtr;

#endif // IGROUPID_H
