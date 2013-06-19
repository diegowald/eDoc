#ifndef IDOCID_H
#define IDOCID_H

#include "eDoc-API_global.h"

struct EDOCAPISHARED_EXPORT IDocID
{
    virtual QString asString() const = 0;
};

#endif // IDOCID_H
