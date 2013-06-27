#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "IDocID.h"
#include <QByteArray>
#include <QtPlugin>

struct IDocument
{
public:
    virtual IDocID* id() = 0;
    virtual QByteArray blob() = 0;
    virtual ~IDocument() {}
};

#endif // IDOCUMENT_H
