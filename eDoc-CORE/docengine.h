#ifndef DOCENGINE_H
#define DOCENGINE_H

#include <QObject>
#include <IDocEngine.h>
#include "document.h"
#include "docid.h"

#include <QMap>

class DocEngine : public QObject, public IDocEngine
{
    Q_OBJECT
public:
    explicit DocEngine(QObject *parent = 0);

    virtual IDocID* addDocument();
    virtual IDocument* getDocument(IDocID *id) const;
    virtual bool deleteDocument(IDocID *id);
    
signals:
    
public slots:

private:
    QMap<DocID, Document *> collection;
};

#endif // DOCENGINE_H
