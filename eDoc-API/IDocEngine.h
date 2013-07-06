#ifndef IDOCENGINE_H
#define IDOCENGINE_H

#include <QtPlugin>
#include "IDocID.h"
#include "IDocBase.h"
#include "IMultiDocument.h"
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include <QMap>

struct IDocEngine
{
public:
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock) = 0;
    virtual IDocID* addDocument(const QByteArray& blob) = 0;
    virtual IDocBase* getDocument(IDocID *id) = 0;
    virtual bool deleteDocument(IDocID *id) = 0;
    virtual QString name() = 0;
    virtual ~IDocEngine() {}
};

Q_DECLARE_INTERFACE(IDocEngine, "com.mksingenieria.eDoc.IDocEngine/0.0")

#endif // IDOCENGINE_H
