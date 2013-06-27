#ifndef FILEENGINE_H
#define FILEENGINE_H

#include <IDocEngine.h>

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

    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocument* getDocument(IDocID *id) const;
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();
};

#endif // FILEENGINE_H
