#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <boost/shared_ptr.hpp>
#include <../eDoc-Configuration/IXMLContent.h>
#include <../eDoc-Configuration/qobjectlgging.h>
#include <QVariant>

typedef QMap<QString, QVariant> DBRecord;
typedef boost::shared_ptr<DBRecord> DBRecordPtr;
typedef boost::shared_ptr<QList<DBRecordPtr> > DBRecordSet;

enum DBType
{
    INVALID_DB_TYPE,
    DB2,
    IBASE,
    MYSQL,
    OCI,
    ODBC,
    PSQL,
    SQLITE,
    SQLITE2,
    TDS
};

class SQLManager : public QObject
{
    Q_OBJECT
public:
    explicit SQLManager(QObject *parent = 0);
    virtual ~SQLManager();
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);

    virtual DBRecordSet getRecords(const QString &sql, DBRecordPtr record);
    virtual void executeCommand(const QString &sql, DBRecordPtr record);
    
private:
    bool tryReconnect();
    DBType String2DBType();
    void addParameters(QSqlQuery &query, const QString &SQL, DBRecordPtr record);
signals:
    
public slots:

private:
    QString m_database;
    QSqlDatabase db;
    QString m_Server;
    QString m_User;
    QString m_Password;
    QObjectLogging *m_Logger;
    DBType m_DBType;
};

#endif // SQLMANAGER_H
