#ifndef QOBJECTLGGING_H
#define QOBJECTLGGING_H

#include <QObject>

class QObjectLgging : public QObject
{
    Q_OBJECT
public:
    explicit QObjectLgging(QObject *parent = 0);
    
signals:
    void LogTrace(const QString& text);
    void LogDebug(const QString& text);
    void LogInfo(const QString& text);
    void LogWarning(const QString& text);
    void LogError(const QString& text);
    void LogFatal(const QString& text);
public slots:
    
};

#endif // QOBJECTLGGING_H
