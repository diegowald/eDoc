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

    virtual void initialize(IXMLContent *configuration,
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
private:
    QSharedPointer<IDocEngine> createPersistentEngine(XMLCollection *confEngine,
                                       const QMap<QString, QString> &docpluginStock,
                                       const QMap<QString, QString> &DBplugins,
                                       const QMap<QString, QString> &DBWithHistoryPlugins,
                                       const QMap<QString, QString> &tagPlugins,
                                       const QMap<QString, QString> &serverPlugins);

private:
     QSharedPointer<IDocEngine> persistentEngine;
     QMap<QString, QSharedPointer<IDocBase>> m_Cache;
     int maxCachedFiles;
     QSharedPointer<QObjectLogging> m_Logger;

};

#endif // MEMORYDOCENGINE_H
