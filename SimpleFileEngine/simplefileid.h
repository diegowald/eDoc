#ifndef SIMPLEFILEID_H
#define SIMPLEFILEID_H

#include <QObject>
#include "../eDoc-API/IDocID.h"
#include <QUuid>

class SimpleFileID : public QObject, public IDocID
{
    Q_OBJECT
public:
    explicit SimpleFileID(QObject *parent = 0);
    SimpleFileID(const QString &stringID, QObject *parent = 0);
    virtual ~SimpleFileID();
    virtual QString asString() const;
    
signals:
    
public slots:
    
private:
    QUuid m_ID;
};

#endif // SIMPLEFILEID_H
