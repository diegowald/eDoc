#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "eDoc-API_global.h"
#include "IDocID.h"

struct EDOCAPISHARED_EXPORT IDocument
{
    virtual IDocID& id() = 0;
    virtual QByteArray blob() = 0;
};

#endif // IDOCUMENT_H
