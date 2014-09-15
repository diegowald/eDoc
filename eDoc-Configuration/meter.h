#ifndef METER_H
#define METER_H

#include <QObject>
#include "edoc-configuration_global.h"
//#include <sys/time.h>
class EDOCCONFIGURATIONSHARED_EXPORT meter : public QObject
{
    Q_OBJECT
public:
    explicit meter(QObject *parent, const QString &methodName);
    meter(const QString &className, const QString &methodName);
    virtual ~meter();
signals:
    
public slots:
private:
    QString meterClassName;
    QString meterFunctionName;
//    timespec ts;
};

#endif // METER_H
