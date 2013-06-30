#ifndef MEMORYDOCENGINE_H
#define MEMORYDOCENGINE_H


#include <IDocEngine.h>



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

    virtual void initialize(IXMLContent *configuration, QObjectLgging *logger);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocument* getDocument(IDocID *id) const;
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();

};

#endif // MEMORYDOCENGINE_H
