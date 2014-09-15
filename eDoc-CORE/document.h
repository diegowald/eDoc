#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <IDocument.h>
#include "docid.h"

CONSTRUCT(Document)

class Document : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit Document(DocIDPtr id, const QString &sourcePath, QObject *parent = 0);
    Document(DocIDPtr id, QObject *parent = 0);
    virtual ~Document();

    virtual IDocIDPtr id();
    virtual QByteArray blob();
    virtual long size();

signals:

public slots:

private:
    DocIDPtr iid;
    QString originalSourcePath;
};

#endif // DOCUMENT_H
