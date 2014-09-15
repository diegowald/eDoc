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

    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob);
    virtual IDocBasePtr createDocument(const QByteArray& blob);
    virtual IDocBasePtr getDocument(QSharedPointer<IDocID> id);
    virtual IDocBasePtr getDocument(const QString &id);
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
