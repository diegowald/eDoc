#ifndef IDOCBASE_H
#define IDOCBASE_H

#include "IDocID.h"

struct IDocBase
{
public:
    virtual IDocID* id() = 0;
    virtual bool isComplex() const = 0;
    virtual ~IDocBase() {}
};

#endif // IDOCBASE_H
