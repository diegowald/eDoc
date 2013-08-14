#include "meter.h"
#include <QDebug>

meter::meter(QObject *parent, const QString &methodName) :
    QObject(parent)
{
    meterClassName = parent->metaObject()->className();
    meterFunctionName = methodName;
    clock_gettime(CLOCK_REALTIME, &ts);
}

meter::meter(const QString &className, const QString &methodName)
{
    meterClassName = className;
    meterFunctionName = methodName;
    clock_gettime(CLOCK_REALTIME, &ts);
    qDebug() << ts.tv_sec;
    qDebug() << ts.tv_nsec;
}

meter::~meter()
{
    timespec tsFinal;
    clock_gettime(CLOCK_REALTIME, &tsFinal);
    qDebug() << tsFinal.tv_sec;
    qDebug() << tsFinal.tv_nsec;
}
