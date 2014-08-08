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

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocument* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual IDocID* IValueToIDocId(IValue* value);
    virtual QString name();

    
signals:
    
public slots:

private:
    QMap<DocID, Document *> collection;
};

#endif // DOCENGINE_H
