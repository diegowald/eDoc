#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "IDocID.h"
#include <QByteArray>
#include <QtPlugin>
#include <IDocBase.h>

struct IDocument : public IDocBase
{
public:
    virtual QByteArray blob() = 0;
    virtual bool isComplex() const { return false; }
    virtual ~IDocument() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IDocument>)

#endif // IDOCUMENT_H
