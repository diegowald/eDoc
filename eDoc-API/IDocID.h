#ifndef IDOCID_H
#define IDOCID_H

#include <QMetaType>
#include <QString>
#include <QSharedPointer>

struct IDocID
{
public:
    virtual QString asString() const = 0;
    virtual ~IDocID() {}
};

typedef QSharedPointer<IDocID> IDocIDPtr;
Q_DECLARE_METATYPE(IDocIDPtr)

#endif // IDOCID_H
