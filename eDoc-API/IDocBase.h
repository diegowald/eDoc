#ifndef IDOCBASE_H
#define IDOCBASE_H
#include <QMetaType>
#include "forward.h"
#include "IDocID.h"

class IDocBase
{
public:
    virtual IDocIDPtr id() = 0;
    virtual bool isComplex() const = 0;
    virtual long size() = 0;
    virtual ~IDocBase() {}
};


Q_DECLARE_METATYPE(IDocBasePtr)

#endif // IDOCBASE_H
