#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include "../eDoc-API/forward.h"
#include <QSharedPointer>
#include "sqlmanager_global.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSharedPointer>
#include <QMap>
#include <../eDoc-Configuration/IXMLContent.h>
#include <../eDoc-Configuration/qobjectlgging.h>
#include <QVariant>

typedef QMap<QString, QVariant> DBRecord;
typedef QSharedPointer<DBRecord> DBRecordPtr;
typedef QSharedPointer<QList<DBRecordPtr> > DBRecordSet;

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

class SQLMANAGERSHARED_EXPORT SQLManager : public QObject
{
    Q_OBJECT
public:
    explicit SQLManager(QObject *parent = 0);
    virtual ~SQLManager();
    virtual void initialize(QSharedPointer<IXMLContent> configuration, QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);

    virtual DBRecordSet getRecords(const QString &sql, DBRecordPtr record);
    virtual DBRecordSet getRecords(const QString &sql);
    virtual void executeCommand(const QString &sql, DBRecordPtr record);

    virtual int executeCommandAndReturnId(const QString &sql, DBRecordPtr record);
    virtual void executeBulk(const QString &sql, QList<DBRecordPtr> &records);

    virtual QStringList getDistintValues(const QString &sql, const QList<QPair<QString, QString> >& filter);
    virtual QList<QPair<QString, QString> > getColumnValues(const QString &sql, const QList<QPair<QString, QString> >& filter);

private:
    bool tryReconnect();
    DBType String2DBType();
    void addParameters(QSqlQuery &query, const QString &SQL, DBRecordPtr record);
    void addParameters(QSqlQuery &query, const QString &SQL, const QList<QPair<QString, QString> >& filter);
signals:

public slots:

private:
    QString m_database;
    QSqlDatabase db;
    QString m_Server;
    QString m_User;
    QString m_Password;
    QSharedPointer<QObjectLogging> m_Logger;
    DBType m_DBType;
};

#endif // SQLMANAGER_H
