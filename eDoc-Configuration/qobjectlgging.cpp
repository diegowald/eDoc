#include "qobjectlgging.h"

QObjectLogging::QObjectLogging(QObject *parent) :
    QObject(parent)
{
}

QObjectLogging::~QObjectLogging()
{
}

void QObjectLogging::logTrace(const QString &filename, const int LineNumber, const QString &ModuleName, const QString& text)
{
    QString s("File: %1. Line: %2. Module: %3. %4");
    emit LogTrace(s.arg(filename).arg(LineNumber).arg(ModuleName).arg(text));
}

void QObjectLogging::logDebug(const QString& text)
{
    emit LogDebug(text);
}

void QObjectLogging::logInfo(const QString& text)
{
    emit LogInfo(text);
}

void QObjectLogging::logWarning(const QString& text)
{
    emit LogWarning(text);
}

void QObjectLogging::logError(const QString& text)
{
    emit LogError(text);
}

void QObjectLogging::logFatal(const QString& text)
{
    emit LogFatal(text);
}
