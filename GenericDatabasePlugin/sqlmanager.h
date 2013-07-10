#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <boost

typedef QMap<QString, QVariant> DBRecord;
typedef boost::shared_ptr<Record> DBRecordPtr;
typedef boost::shared_ptr<QList<RecordPtr> > DBRecordSet;

class SQLManager : public QObject
{
    Q_OBJECT
public:
    explicit SQLManager(QObject *parent = 0);
    virtual ~SQLManager();
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);

    virtual DBRecordSet getRecords(const QString &sql, dbRecordPtr record);
    virtual void executeCommand(const QString &sql, dbRecord record);
    
signals:
    
public slots:

private:
    QString m_database;
    QSqlDatabase db;
    QString m_Server;
    QString m_User;
    QString m_Password;

};

#endif // SQLMANAGER_H
