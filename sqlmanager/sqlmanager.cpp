#include "sqlmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include "../eDoc-API/IFactory.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include <QDateTime>

SQLManager::SQLManager(QObject *parent) :
    QObject(parent)
{
}

SQLManager::~SQLManager()
{
}

void SQLManager::initialize(QSharedPointer<IXMLContent> configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins)
{
    (void)docpluginStock;
    (void)DBplugins;
    (void)tagPlugins;
    (void)serverPlugins;

    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_database = configuration.dynamicCast<XMLCollection>()->get("database_type").dynamicCast<XMLElement>()->value();
    m_Server = configuration.dynamicCast<XMLCollection>()->get("server").dynamicCast<XMLElement>()->value();
    m_User = configuration.dynamicCast<XMLCollection>()->get("user").dynamicCast<XMLElement>()->value();
    m_Password = configuration.dynamicCast<XMLCollection>()->get("password").dynamicCast<XMLElement>()->value();

    m_DBType = String2DBType();
}

void SQLManager::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    m_Logger = factory->logger();
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_database = configuration.dynamicCast<XMLCollection>()->get("database_type").dynamicCast<XMLElement>()->value();
    m_Server = configuration.dynamicCast<XMLCollection>()->get("server").dynamicCast<XMLElement>()->value();
    m_User = configuration.dynamicCast<XMLCollection>()->get("user").dynamicCast<XMLElement>()->value();
    m_Password = configuration.dynamicCast<XMLCollection>()->get("password").dynamicCast<XMLElement>()->value();

    m_DBType = String2DBType();
}

DBType SQLManager::String2DBType()
{
    QString dbtype = m_database.trimmed().toLower();

    if ("db2" == dbtype)
        return DB2;
    else if ("ibase" == dbtype)
        return IBASE;
    else if ("mysql" == dbtype)
        return MYSQL;
    else if ("oci" == dbtype)
        return OCI;
    else if ("odbc" == dbtype)
        return ODBC;
    else if ("psql" == dbtype)
        return PSQL;
    else if ("sqlite" == dbtype)
        return SQLITE;
    else if ("sqlite2" == dbtype)
        return SQLITE2;
    else if ("tds" == dbtype)
        return TDS;
    else
        return INVALID_DB_TYPE;
}

DBRecordSet SQLManager::getRecords(const QString &sql, DBRecordPtr record)
{
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DBRecordSet SQLManager::getRecords(const QString &sql, DBRecordPtr record)");
    DBRecordSet response = DBRecordSet(new QList<DBRecordPtr>());

    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return response;
    }

    QSqlQuery q;
    q.prepare(sql);
    addParameters(q, sql, record);
    q.exec();

    while (q.next())
    {
        QSqlRecord rec = q.record();
        DBRecordPtr record = DBRecordPtr(new DBRecord());
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

DBRecordSet SQLManager::getRecords(const QString &sql)
{
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DBRecordSet SQLManager::getRecords(const QString &sql)");
    DBRecordSet response = DBRecordSet(new QList<DBRecordPtr>());

    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return response;
    }

    QSqlQuery q;
    q.prepare(sql);
    q.exec();

    while (q.next())
    {
        QSqlRecord rec = q.record();
        DBRecordPtr record = DBRecordPtr(new DBRecord());
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

void SQLManager::addParameters(QSqlQuery &query, const QString &SQL, DBRecordPtr record)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::addParameters(QSqlQuery &query, QString &SQL, DBRecordPtr record)");
    foreach(QString key, record->keys())
    {
        QString param(":" + key);
        if (SQL.contains(param))
        {
            QVariant value = (*record)[key];
            switch (value.type())
            {
            case QVariant::DateTime:
            case QVariant::Date:
            case QVariant::Time:
            {
                QDateTime dt = value.toDateTime();
                if (value.type() == QVariant::Time)
                {
                    dt.setDate(QDateTime::currentDateTime().date());
                    dt.setTime(value.toTime());
                }
                value = dt.toMSecsSinceEpoch();
                break;
            }
            default:
                break;
            }
          //  m_Logger->logDebug("param: " + param);
            //m_Logger->logDebug("value: " + value.toString());
            query.bindValue(param, value);
        }
    }
}

void SQLManager::addParameters(QSqlQuery &query, const QString &SQL, const QList<QPair<QString, QString> >& filter)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::addParameters(QSqlQuery &query, QString &SQL, DBRecordPtr record)");
    QPair<QString, QString> filterColumn;
    foreach(filterColumn, filter)
    {
        QString param(":" + filterColumn.first);
        if (SQL.contains(param))
        {
            QVariant value = filterColumn.second;
            query.bindValue(param, value);
        }
    }
}

void SQLManager::executeCommand(const QString &sql, DBRecordPtr record)
{
    executeCommandAndReturnId(sql, record);
}

int SQLManager::executeCommandAndReturnId(const QString &sql, DBRecordPtr record)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::executeCommand(const QString &sql, DBRecordPtr record)");
    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return -1;
    }

    QSqlQuery q;
    q.prepare(sql);
    addParameters(q, sql, record);
    q.exec();
    if (q.lastError().type() != QSqlError::NoError)
    {
        m_Logger->logError("SQL Error:" + q.lastError().text());
    }
    int id = q.lastInsertId().toInt();
    db.close();
    return id;
}

void SQLManager::executeBulk(const QString &sql, QList<DBRecordPtr> &records)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void SQLManager::executeCommand(const QString &sql, DBRecordPtr record)");
    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return;
    }

    QSqlQuery q;
    q.prepare(sql);
    db.transaction();
    foreach (DBRecordPtr record, records)
    {
        addParameters(q, sql, record);
        q.exec();
        if (q.lastError().type() != QSqlError::NoError)
        {
            m_Logger->logError("SQL Error:" + q.lastError().text());
        }
    }
    db.commit();
    db.close();
    return;
}

QStringList SQLManager::getDistintValues(const QString &sql, const QList<QPair<QString, QString> >& filter)
{
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "QStringList SQLManager::getDistintValues(const QString &sql, const QStringList& filter)");
    QStringList response;

    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return response;
    }

    QSqlQuery q;
    q.prepare(sql);
    addParameters(q, sql, filter);
    q.exec();

    while (q.next())
    {
        QSqlRecord rec = q.record();
        for (int i = 0; i < rec.count(); i++)
        {
            response.push_back(rec.field(0).value().toString());
        }
    }

    if (q.lastError().type() != QSqlError::NoError)
    {
        m_Logger->logError(QString("SQL Error: %1").arg(q.lastError().text()));
    }
    db.close();
    return response;
}

QList<QPair<QString, QString> > SQLManager::getColumnValues(const QString &sql, const QList<QPair<QString, QString> >& filter)
{
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "QList<QPair<QString, QString> > SQLManager::getColumnValues(const QString &sql, const QList<QPair<QString, QString> >& filter)");

    QList<QPair<QString, QString> > response;

    if (!tryReconnect())
    {
        m_Logger->logError(QString("Can't open Database."
                           " Reason: ") +
                           db.lastError().text());
        // Error
        return response;
    }

    QSqlQuery q;
    q.prepare(sql);
    addParameters(q, sql, filter);
    q.exec();

    while (q.next())
    {
        QSqlRecord rec = q.record();
        for (int i = 0; i < rec.count(); i++)
        {
            QPair<QString, QString> pair;
            pair.first = rec.field(0).value().toString();
            pair.second = rec.field(1).value().toString();
            response.push_back(pair);
        }
    }

    if (q.lastError().type() != QSqlError::NoError)
    {
        m_Logger->logError(QString("SQL Error: %1").arg(q.lastError().text()));
    }
    db.close();
    return response;
}

bool SQLManager::tryReconnect()
{
//    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "bool SQLManager::tryReconnect()");
    if (!db.isOpen() && !db.open())
    {
        switch (m_DBType)
        {
        case DB2:
           // m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case IBASE:
        //    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case MYSQL:
      ////      m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "Database trying to reconnect: MySQL Database");
            db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName(m_Server);
            db.setDatabaseName(m_database);
            db.setUserName(m_User);
            db.setPassword(m_Password);
            break;
        case OCI:
      //      m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case ODBC:
       ///     m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case PSQL:
         //   m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case SQLITE:
       //     m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "Database trying to reconnect: SQLite Database");
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setHostName("localhost");
            db.setDatabaseName(m_Server);
            break;
        case SQLITE2:
      //      m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case TDS:
        //    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "DATABASE TYPE NOT IMPLEMENTED YET");
            break;
        case INVALID_DB_TYPE:
            // Fall through
       //     m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "INVALID DATABASE TYPE");
        default:
            return false;
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
