#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include "../eDoc-API/IDocument.h"

CONSTRUCT(Document)

class Document : public QObject, public IDocument
{
    Q_OBJECT
public:
    explicit Document(IDocIDPtr id, const QString &location, QObject *parent = 0);
    virtual ~Document();
    virtual IDocIDPtr id();
    virtual QByteArray blob();
    virtual long size();

signals:

public slots:

private:
    QString fileLocation;
    IDocIDPtr _id;
};

#endif // DOCUMENT_H
