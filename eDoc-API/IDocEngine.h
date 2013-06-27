#ifndef IDOCENGINE_H
#define IDOCENGINE_H

#include <QtPlugin>
#include "IDocID.h"
#include "IDocument.h"

struct IDocEngine
{
public:
    virtual IDocID* addDocument() = 0;
    virtual IDocument* getDocument(IDocID *id) const = 0;
    virtual bool deleteDocument(IDocID *id) = 0;
    virtual QString name() = 0;
    virtual ~IDocEngine() {}
};

Q_DECLARE_INTERFACE(IDocEngine, "com.mksingenieria.eDoc.IDocEngine/0.0")

#endif // IDOCENGINE_H
