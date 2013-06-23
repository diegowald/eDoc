#ifndef IDOCID_H
#define IDOCID_H

#include "eDoc-API_global.h"
#include <QString>

struct EDOCAPISHARED_EXPORT IDocID
{
public:
    virtual QString asString() const = 0;
};

#endif // IDOCID_H
