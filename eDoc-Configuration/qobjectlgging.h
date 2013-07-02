#ifndef QOBJECTLGGING_H
#define QOBJECTLGGING_H
#include "edoc-configuration_global.h"
#include <QObject>

class EDOCCONFIGURATIONSHARED_EXPORT QObjectLogging : public QObject
{
    Q_OBJECT
public:
    explicit QObjectLogging(QObject *parent = 0);
    virtual ~QObjectLogging();

    virtual void logTrace(const QString& text);
    virtual void logDebug(const QString& text);
    virtual void logInfo(const QString& text);
    virtual void logWarning(const QString& text);
    virtual void logError(const QString& text);
    virtual void logFatal(const QString& text);

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
