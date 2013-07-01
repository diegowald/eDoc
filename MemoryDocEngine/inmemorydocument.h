#ifndef INMEMORYDOCUMENT_H
#define INMEMORYDOCUMENT_H

#include <QObject>
#include "../eDoc-API/IDocument.h"

class InMemoryDocument : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit InMemoryDocument(IDocument* persistentDoc, QObject *parent = 0);
    virtual ~InMemoryDocument();
    virtual IDocID* id();
    virtual QByteArray blob();
signals:
    
public slots:

private:
    QByteArray cachedData;
    bool cached;
    IDocument *m_PersistentDocument;
};

#endif // INMEMORYDOCUMENT_H
