#ifndef IDOCENGINE_H
#define IDOCENGINE_H

#include <QtPlugin>
#include "forward.h"
#include "IInitializable.h"
#include "IDocID.h"
#include "IDocBase.h"
#include "IMultiDocument.h"
#include "IValue.h"
#include <QMap>


struct IDocEngine : public IInitializable
{
public:
    //virtual IDocBasePtr newDocument() = 0;
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob) = 0;
    virtual IDocBasePtr createDocument(const QByteArray& blob) = 0;
    virtual IDocBasePtr getDocument(IDocIDPtr id) = 0;
    virtual IDocBasePtr getDocument(const QString &id) = 0;
    virtual bool deleteDocument(IDocIDPtr id) = 0;
    virtual IDocIDPtr IValueToIDocId(IValuePtr value) = 0;
    virtual QString name() = 0;
    virtual IDocEnginePtr newDocEngine() = 0;
    virtual ~IDocEngine() {}
};

Q_DECLARE_INTERFACE(IDocEngine, "com.mksingenieria.eDoc.IDocEngine/0.0")

#endif // IDOCENGINE_H
