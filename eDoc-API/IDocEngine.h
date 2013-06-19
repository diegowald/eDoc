#ifndef IDOCENGINE_H
#define IDOCENGINE_H

#include "eDoc-API_global.h"
#include "IDocID.h"
#include "IDocument.h"

class EDOCAPISHARED_EXPORT IDocEngine
{
public:
    virtual IDocID* addDocument() = 0;
    virtual IDocument* getDocument(IDocID &id) const = 0;
    virtual bool deleteDocument(IDocID &id) = 0;
    //IDocEngine();
};

#endif // IDOCENGINE_H
