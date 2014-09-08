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

    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);
    virtual QSharedPointer<IDocID> addDocument(const QByteArray& blob);
    virtual QSharedPointer<IDocBase> getDocument(QSharedPointer<IDocID> id);
    virtual bool deleteDocument(QSharedPointer<IDocID> id);
    virtual QSharedPointer<IDocID> IValueToIDocId(QSharedPointer<IValue> value);
    virtual QString name();

    virtual IDocEnginePtr newDocEngine();

private:
    QString folder;
    QSharedPointer<QObjectLogging> m_Logger;
    QSharedPointer<FileManagement> fileManager;
};

#endif // FILEENGINE_H
