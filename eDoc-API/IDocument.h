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

Q_DECLARE_INTERFACE(IDocument, "com.mksingenieria.eDoc.IDocument/0.0")
#endif // IDOCUMENT_H
