#include "sqlmanager.h"

SQLManager::SQLManager(QObject *parent) :
    QObject(parent)
{
}

SQLManager::~SQLManager()
{
}

void SQLManager::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    QString m_database;
    QSqlDatabase db;
    QString m_Server;
    QString m_User;
    QString m_Password;
}

DBRecordSet SQLManager::getRecords(const QString &sql, dbRecordPtr record)
{

}

void SQLManager::executeCommand(const QString &sql, dbRecord record)
{

}
