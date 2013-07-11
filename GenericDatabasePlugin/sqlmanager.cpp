#include "sqlmanager.h"
#include <QSqlQuery>
#include <boost/make_shared.hpp>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"


SQLManager::SQLManager(QObject *parent) :
    QObject(parent)
{
}

SQLManager::~SQLManager()
{
}

void SQLManager::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    m_Logger = logger;
    m_Logger->logTrace("void SQLManager::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_database = ((XMLElement*)((XMLCollection*) configuration)->get("database"))->value();
    m_Server = ((XMLElement*)((XMLCollection*) configuration)->get("server"))->value();
    m_User = ((XMLElement*)((XMLCollection*) configuration)->get("user"))->value();
    m_Password = ((XMLElement*)((XMLCollection*) configuration)->get("password"))->value();

    /*m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    XMLCollection *confFields = (XMLCollection*)((XMLCollection*)configuration)->get("fields");
    */
}

DBRecordSet SQLManager::getRecords(const QString &sql, DBRecordPtr record)
{
    m_Logger->logTrace("DBRecordSet SQLManager::getRecords(const QString &sql, DBRecordPtr record)");
    DBRecordSet response = boost::make_shared<QList<DBRecordPtr> >();

    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return response;
    }

    QSqlQuery q(sql);
    q.exec();

    while (q.next())
    {
        QSqlRecord rec = q.record();
        DBRecordPtr record = boost::make_shared<DBRecord>();
        for (int i = 0; i < rec.count(); i++)
        {
            (*record)[rec.fieldName(i)] = rec.field(i).value();
        }
        response->push_back(record);
    }

    if (q.lastError().type() != QSqlError::NoError)
    {
        m_Logger->logError(QString("SQL Error: %1").arg(q.lastError().text()));
    }
    db.close();
    return response;
}

void SQLManager::executeCommand(const QString &sql, DBRecord record)
{

}

bool SQLManager::tryReconnect()
{
    m_Logger->logTrace("bool SQLManager::tryReconnect()");
    if (!db.isOpen() && !db.open())
    {
        if (m_UsingSQLITE)
        {
            QLOG_TRACE() << "Database trying to reconnect: SQLite Database";
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setHostName("localhost");
            db.setDatabaseName(m_database);
        }
        else
        {
            QLOG_TRACE() << "Database trying to reconnect: MySQL Database";
            db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName(m_Server);
            db.setDatabaseName(m_database);
            db.setUserName(m_User);
            db.setPassword(m_Password);
        }
    }


    if (db.isOpen())
        return true;

    bool result = db.open();
    if (!result)
    {
        m_Logger->logError(QString("DB cannot be opened. Reason: %1").arg(db.lastError().text()));
    }
    return result;
}
