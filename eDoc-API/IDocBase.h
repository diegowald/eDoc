#ifndef IDOCBASE_H
#define IDOCBASE_H

#include <QMetaType>

#include "IDocID.h"

struct IDocBase
{
public:
    virtual IDocIDPtr id() = 0;
    virtual bool isComplex() const = 0;
    virtual ~IDocBase() {}
};

typedef QSharedPointer<IDocBase> IDocBasePtr;

Q_DECLARE_METATYPE(IDocBasePtr)

#endif // IDOCBASE_H
