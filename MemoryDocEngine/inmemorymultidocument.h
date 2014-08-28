#ifndef INMEMORYMULTIDOCUMENT_H
#define INMEMORYMULTIDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IMultiDocument.h"
#include <QMap>

class InMemoryMultiDocument : public QObject, public IMultiDocument
{
    Q_OBJECT
public:    
    explicit InMemoryMultiDocument(QSharedPointer<IMultiDocument> persistentDoc, QObject *parent = 0);
    virtual ~InMemoryMultiDocument();

    virtual QSharedPointer<IDocID> id();
    virtual void addDocument(QSharedPointer<IDocBase> doc);
    virtual QSharedPointer<IDocBase> getDocument(QSharedPointer<IDocID> id);
    virtual QList<QSharedPointer<IDocBase>> getDocuments();
    virtual void removeDocument(QSharedPointer<IDocID> id);
    virtual bool containsDocument(QSharedPointer<IDocID> id);
signals:
    
public slots:
    
private:
    QByteArray cachedData;
    bool cached;
    QSharedPointer<IMultiDocument> m_PersistentDocument;
    QMap<QString, QSharedPointer<IDocBase>> m_Collection;
};

#endif // INMEMORYMULTIDOCUMENT_H
