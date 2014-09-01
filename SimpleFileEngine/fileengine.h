#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <IDocEngine.h>
#include "filemanagement.h"

class FileEngine : public QObject, public IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "SimpleFileEngine.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)

    
public:
    FileEngine(QObject *parent = 0);
    virtual ~FileEngine();

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

private:
    QString folder;
    QSharedPointer<QObjectLogging> m_Logger;
    QSharedPointer<FileManagement> fileManager;
};

#endif // FILEENGINE_H
