#ifndef DOCENGINE_H
#define DOCENGINE_H


#include <QObject>
#include <QtCore/QtPlugin>
#include <IDocEngine.h>
#include "document.h"
#include "docid.h"

#include <QMap>

class DocEngine : public QObject, virtual public IDocEngine
{
    Q_OBJECT
    Q_INTERFACES(IDocEngine)

public:
    explicit DocEngine(QObject *parent = 0);
    virtual ~DocEngine();

    virtual IDocID* addDocument();
    virtual IDocument* getDocument(IDocID *id) const;
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();
    
signals:
    
public slots:

private:
    QMap<DocID, Document *> collection;
};

#endif // DOCENGINE_H
