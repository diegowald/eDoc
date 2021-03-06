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
    virtual IDocBasePtr createDocument(const QByteArray& blob);
    virtual IDocBasePtr createDocument(const QString sourcePath, const QByteArray &blob);
    virtual IDocBasePtr getDocument(IDocIDPtr id);
    virtual IDocBasePtr getDocument(const QString &id);
    virtual bool deleteDocument(IDocIDPtr id);
    virtual IDocIDPtr IValueToIDocId(IValuePtr value);
    virtual QString name();

    virtual IDocEnginePtr newDocEngine();

private:
    QString folder;
    QObjectLoggingPtr m_Logger;
    QSharedPointer<FileManagement> fileManager;
};

#endif // FILEENGINE_H
