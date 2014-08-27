#ifndef MEMORYDOCENGINE_H
#define MEMORYDOCENGINE_H


#include <IDocEngine.h>
#include <QMap>
#include <inmemorydocument.h>
#include <inmemorymultidocument.h>
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

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocBase* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual IDocID* IValueToIDocId(IValue *value);
    virtual QString name();
private:
    IDocEngine *createPersistentEngine(XMLCollection *confEngine,
                                       const QMap<QString, QString> &docpluginStock,
                                       const QMap<QString, QString> &DBplugins,
                                       const QMap<QString, QString> &DBWithHistoryPlugins,
                                       const QMap<QString, QString> &tagPlugins,
                                       const QMap<QString, QString> &serverPlugins);

private:
     IDocEngine *persistentEngine;
     QMap<QString, IDocBase*> m_Cache;
     int maxCachedFiles;
     QObjectLogging *m_Logger;

};

#endif // MEMORYDOCENGINE_H
