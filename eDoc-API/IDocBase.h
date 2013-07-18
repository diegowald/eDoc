#ifndef IDOCBASE_H
#define IDOCBASE_H

#include <QMetaType>

#include "IDocID.h"

struct IDocBase
{
public:
    virtual IDocID* id() = 0;
    virtual bool isComplex() const = 0;
    virtual ~IDocBase() {}
};

Q_DECLARE_METATYPE(IDocBase*)

#endif // IDOCBASE_H
