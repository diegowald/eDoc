#ifndef PROXYDOCID_H
#define PROXYDOCID_H

#include <QObject>
#include "../eDoc-API/IDocID.h"

class ProxyDocID : public QObject, public IDocID
{
    Q_OBJECT
public:
    explicit ProxyDocID(QObject *parent = 0);
    virtual ~ProxyDocID();
    virtual QString asString() const;

    friend QDataStream& operator>>(QDataStream& is, ProxyDocID &obj);
signals:

public slots:

private:
    QString id;
};

#endif // PROXYDOCID_H
