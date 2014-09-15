#ifndef MEMORYDOCENGINE_H
#define MEMORYDOCENGINE_H


#include <IDocEngine.h>
#include <QMap>
#include <inmemorydocument.h>
#include <inmemorymultidocument.h>
#include <../eDoc-Configuration/xmlcollection.h>

class MemoryDocEngine : public QObject, public IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "MemoryDocEngine.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)

public:
    MemoryDocEngine(QObject *parent = 0);
    virtual ~MemoryDocEngine();

    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob);
    virtual IDocBasePtr createDocument(const QByteArray& blob);
    virtual IDocBasePtr getDocument(IDocIDPtr id);
    virtual IDocBasePtr getDocument(const QString &id);
    virtual bool deleteDocument(IDocIDPtr id);
    virtual IDocIDPtr IValueToIDocId(IValuePtr value);
    virtual QString name();
    virtual IDocEnginePtr newDocEngine();

private:
     IDocEnginePtr persistentEngine;
     QMap<QString, IDocBasePtr> m_Cache;
     int maxCachedFiles;

     QObjectLoggingPtr m_Logger;
};

#endif // MEMORYDOCENGINE_H
