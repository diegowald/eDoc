#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <IDocument.h>
#include "docid.h"

class Document : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit Document(DocID* id, QObject *parent = 0);

    virtual IDocID& id();
    virtual QByteArray blob();

signals:
    
public slots:
    
private:
    DocID *iid;
};

#endif // DOCUMENT_H
