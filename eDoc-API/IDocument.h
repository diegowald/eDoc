#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "forward.h"
#include "IDocID.h"
#include <QByteArray>
#include <QtPlugin>
#include "IDocBase.h"

class IDocument : public IDocBase
{
public:
    virtual QByteArray blob() = 0;
    virtual bool isComplex() const { return false; }
    virtual ~IDocument() {}
};


Q_DECLARE_METATYPE(IDocumentPtr)

#endif // IDOCUMENT_H
