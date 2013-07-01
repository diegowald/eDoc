#include "qobjectlgging.h"

QObjectLgging::QObjectLgging(QObject *parent) :
    QObject(parent)
{
}

QObjectLgging::~QObjectLgging()
{
}

void QObjectLgging::logTrace(const QString& text)
{
    emit LogTrace(text);
}

void QObjectLgging::logDebug(const QString& text)
{
    emit LogDebug(text);
}

void QObjectLgging::logInfo(const QString& text)
{
    emit LogInfo(text);
}

void QObjectLgging::logWarning(const QString& text)
{
    emit LogWarning(text);
}

void QObjectLgging::logError(const QString& text)
{
    emit LogError(text);
}

void QObjectLgging::logFatal(const QString& text)
{
    emit LogFatal(text);
}
