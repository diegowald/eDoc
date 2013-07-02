#ifndef MEMORYDOCENGINE_H
#define MEMORYDOCENGINE_H


#include <IDocEngine.h>
#include <QMap>
#include <inmemorydocument.h>
#include <../eDoc-Configuration/xmlcollection.h>

class MemoryDocEngine : public QObject, IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "MemoryDocEngine.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)

public:
    MemoryDocEngine(QObject *parent = 0);
    virtual ~MemoryDocEngine();

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocument* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();
private:
    IDocEngine *createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock);

private:
     IDocEngine *persistentEngine;
     QMap<QString, InMemoryDocument*> m_Cache;
     int maxCachedFiles;
     QObjectLogging *m_Logger;

};

#endif // MEMORYDOCENGINE_H
