#ifndef IMULTIDOCUMENT_H
#define IMULTIDOCUMENT_H


#include "IDocID.h"
#include <QtPlugin>
#include "IDocBase.h"

struct IMultiDocument: public IDocBase
{
public:
    virtual bool isComplex() const { return true; }
    virtual void addDocument(IDocBasePtr id) = 0;
    virtual QList<IDocBasePtr> getDocuments() = 0;
    virtual IDocBasePtr getDocument(IDocIDPtr id) = 0;
    virtual void removeDocument(IDocIDPtr id) = 0;
    virtual bool containsDocument(IDocIDPtr id) = 0;
    virtual ~IMultiDocument() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IMultiDocument>)

#endif // IMULTIDOCUMENT_H
