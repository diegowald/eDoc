#ifndef DOCENGINE_H
#define DOCENGINE_H


#include <QObject>
#include <QtPlugin>
#include <IDocEngine.h>
#include "document.h"
#include "docid.h"

#include <QMap>

class DocEngine : public QObject, IDocEngine
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "DocEngine.json")
    Q_INTERFACES(IDocEngine)

public:
    explicit DocEngine(QObject *parent = 0);
    virtual ~DocEngine();

    virtual void initialize(QSharedPointer<IXMLContent> configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QSharedPointer<IDocID> addDocument(const QByteArray& blob);
    virtual QSharedPointer<IDocBase> getDocument(QSharedPointer<IDocID> id);
    virtual bool deleteDocument(QSharedPointer<IDocID> id);
    virtual QSharedPointer<IDocID> IValueToIDocId(QSharedPointer<IValue> value);
    virtual QString name();

    virtual IDocEnginePtr newDocEngine();
    
signals:
    
public slots:

private:
    QMap<DocID, QSharedPointer<Document>> collection;
};

#endif // DOCENGINE_H
