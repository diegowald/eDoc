#ifndef DOCID_H
#define DOCID_H

#include <QObject>
#include <IDocID.h>
#include "../eDoc-API/forward.h"

CONSTRUCT(DocID)

class DocID : public IDocID
{
public:
    explicit DocID(QString id);
    
    virtual QString asString() const;

    inline bool operator ==(const DocID & other)
    {
        return (value == other.value);
    }

    inline bool operator <(const DocID & other) const
    {
        return (value < other.value);
    }

signals:
    
public slots:
    
public:
    static DocID *createNew();

private:
    QString value;
};

#endif // DOCID_H
