#ifndef IMULTIDOCUMENT_H
#define IMULTIDOCUMENT_H


#include "IDocID.h"
#include <QtPlugin>
#include "IDocBase.h"

struct IMultiDocument: public IDocBase
{
public:
    virtual bool isComplex() const { return true; }
    virtual void addDocument(IDocBase *id) = 0;
    virtual QList<IDocBase*> getDocuments() = 0;
    virtual IDocBase* getDocument(IDocID *id) = 0;
    virtual void removeDocument(IDocID *id) = 0;
    virtual bool containsDocument(IDocID *id) = 0;
    virtual ~IMultiDocument() {}
};

Q_DECLARE_METATYPE(IMultiDocument*)

#endif // IMULTIDOCUMENT_H
