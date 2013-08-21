#ifndef TASK_H
#define TASK_H

#include <QObject>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QString &appPath, QObject *parent = 0);

signals:
    void finished();
public slots:
    void run();

private:
    EDocFactory f;
    QString m_ApplicationPath;
    QObjectLogging logger;
};

#endif // TASK_H
