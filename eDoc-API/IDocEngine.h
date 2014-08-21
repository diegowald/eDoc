#ifndef IDOCENGINE_H
#define IDOCENGINE_H

#include <QtPlugin>
#include "IInitializable.h"
#include "IDocID.h"
#include "IDocBase.h"
#include "IMultiDocument.h"
#include "IValue.h"
#include <QMap>

struct IDocEngine : public IInitializable
{
public:
    virtual IDocID* addDocument(const QByteArray& blob) = 0;
    virtual IDocBase* getDocument(IDocID *id) = 0;
    virtual bool deleteDocument(IDocID *id) = 0;
    virtual IDocID* IValueToIDocId(IValue* value) = 0;
    virtual QString name() = 0;
    virtual ~IDocEngine() {}
};

Q_DECLARE_INTERFACE(IDocEngine, "com.mksingenieria.eDoc.IDocEngine/0.0")

#endif // IDOCENGINE_H
