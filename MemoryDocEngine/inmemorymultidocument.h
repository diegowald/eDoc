#ifndef INMEMORYMULTIDOCUMENT_H
#define INMEMORYMULTIDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IMultiDocument.h"
#include <QMap>

class InMemoryMultiDocument : public QObject, public IMultiDocument
{
    Q_OBJECT
public:    
    explicit InMemoryMultiDocument(IMultiDocument* persistentDoc, QObject *parent = 0);
    virtual ~InMemoryMultiDocument();

    virtual IDocID* id();
    virtual void addDocument(IDocBase *doc);
    virtual IDocBase* getDocument(IDocID *id);
    virtual QList<IDocBase*> getDocuments();
    virtual void removeDocument(IDocID *id);
    virtual bool containsDocument(IDocID *id);
signals:
    
public slots:
    
private:
    QByteArray cachedData;
    bool cached;
    IMultiDocument *m_PersistentDocument;
    QMap<QString, IDocBase*> m_Collection;
};

#endif // INMEMORYMULTIDOCUMENT_H
