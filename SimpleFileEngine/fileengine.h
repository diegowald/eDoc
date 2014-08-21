#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <IDocEngine.h>
#include "filemanagement.h"

class FileEngine : public QObject, IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "SimpleFileEngine.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)

    
public:
    FileEngine(QObject *parent = 0);
    virtual ~FileEngine();

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocBase* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual IDocID* IValueToIDocId(IValue *value);
    virtual QString name();

private:
    QString folder;
    QObjectLogging *m_Logger;
    FileManagement *fileManager;
};

#endif // FILEENGINE_H
