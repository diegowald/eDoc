#ifndef INMEMORYDOCUMENT_H
#define INMEMORYDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IDocument.h"

class InMemoryDocument : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit InMemoryDocument(QSharedPointer<IDocument> persistentDoc, QObject *parent = 0);
    virtual ~InMemoryDocument();
    virtual QSharedPointer<IDocID> id();
    virtual QByteArray blob();
    virtual long size();
signals:
    
public slots:

private:
    QByteArray cachedData;
    bool cached;
    QSharedPointer<IDocument> m_PersistentDocument;
};

#endif // INMEMORYDOCUMENT_H
