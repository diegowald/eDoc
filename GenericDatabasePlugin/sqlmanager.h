#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <boost/shared_ptr.hpp>
#include <../eDoc-Configuration/IXMLContent.h>
#include <../eDoc-Configuration/qobjectlgging.h>

typedef QMap<QString, QVariant> DBRecord;
typedef boost::shared_ptr<DBRecord> DBRecordPtr;
typedef boost::shared_ptr<QList<DBRecordPtr> > DBRecordSet;

class SQLManager : public QObject
{
    Q_OBJECT
public:
    explicit SQLManager(QObject *parent = 0);
    virtual ~SQLManager();
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);

    virtual DBRecordSet getRecords(const QString &sql, DBRecordPtr record);
    virtual void executeCommand(const QString &sql, DBRecord record);
    
private:
    bool tryReconnect();
signals:
    
public slots:

private:
    QString m_database;
    QSqlDatabase db;
    QString m_Server;
    QString m_User;
    QString m_Password;
    QObjectLogging *m_Logger;
};

#endif // SQLMANAGER_H
