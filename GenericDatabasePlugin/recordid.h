#ifndef RECORDID_H
#define RECORDID_H

#include <QObject>
#include "../eDoc-API/IRecordID.h"
#include <QUuid>

class RecordID : public QObject, public IRecordID
{
    Q_OBJECT
public:
    explicit RecordID(QObject *parent = 0);
    RecordID(const QString &stringID, QObject *parent = 0);
    virtual ~RecordID();
    virtual QString asString() const;
signals:

public slots:
private:
    QUuid m_ID;
};


#endif // RECORDID_H
