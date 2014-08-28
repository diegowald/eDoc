#ifndef IMULTIDOCUMENT_H
#define IMULTIDOCUMENT_H


#include "IDocID.h"
#include <QtPlugin>
#include "IDocBase.h"

struct IMultiDocument: public IDocBase
{
public:
    virtual bool isComplex() const { return true; }
    virtual void addDocument(QSharedPointer<IDocBase> id) = 0;
    virtual QList<QSharedPointer<IDocBase> > getDocuments() = 0;
    virtual QSharedPointer<IDocBase> getDocument(QSharedPointer<IDocID> id) = 0;
    virtual void removeDocument(QSharedPointer<IDocID> id) = 0;
    virtual bool containsDocument(QSharedPointer<IDocID> id) = 0;
    virtual ~IMultiDocument() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IMultiDocument>)

#endif // IMULTIDOCUMENT_H
