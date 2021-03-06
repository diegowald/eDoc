#ifndef EDOCTCPHISTORICCLIENT_H
#define EDOCTCPHISTORICCLIENT_H

#include "edoctcphistoricclient_global.h"
#include <QObject>
#include "../eDoc-API/IDatabaseWithHistory.h"
#include "../eDocTCPMessages/tcpclient.h"

class EDOCTCPHISTORICCLIENTSHARED_EXPORT EDocTcpHistoricClient : virtual public TcpClient, virtual public IDatabaseWithHistory
{
    Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabase" FILE "eDocTcpHistoricClient.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabaseWithHistory)

public:
    EDocTcpHistoricClient(QObject *parent = 0);
    virtual ~EDocTcpHistoricClient();

    //IInitializable
public:
    virtual void initialize(IXMLContentPtr configuration, IFactory* factory);

    //IDatabase
    virtual QList<QSharedPointer<IFieldDefinition> > fields();
    virtual QSharedPointer<IFieldDefinition> field(const QString &fieldName);
    virtual QSharedPointer<IParameter> createEmptyParameter();
    virtual QList<QSharedPointer<IRecordID> > search(const QList<QSharedPointer<IParameter> > &parameters);
    virtual QList<QSharedPointer<IRecordID> > searchWithin(const QList<QSharedPointer<IParameter> > &parameters, const QList<QSharedPointer<IRecordID> > &records);
    virtual QSharedPointer<IRecord> createEmptyRecord();
    virtual QSharedPointer<IRecordID> addRecord(QSharedPointer<IRecord> record);
    virtual QSharedPointer<IRecord> getRecord(QSharedPointer<IRecordID> id);
    virtual QSharedPointer<IRecord> getRecord(const QString &id);
    virtual QList<QSharedPointer<IRecord> > getRecords(const QStringList &ids);
    virtual void updateRecord(QSharedPointer<IRecord> record);
    virtual void deleteRecord(QSharedPointer<IRecordID> id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QList<QPair<QString, QString>> getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();
    virtual IDatabasePtr newDatabase();
    virtual IDatabaseWithHistoryPtr newDatabaseWithHistory();

protected:
    virtual QMap<QString, QSharedPointer<IRecordID> > search(QSharedPointer<IParameter> parameter);

//IDatabaseWithHistory
public:
    virtual QList<QSharedPointer<IRecordID> > searchByDate(const QList<QSharedPointer<IParameter> > &parameters, const QDateTime &date);
    virtual QSharedPointer<IRecord> getRecordByDate(QSharedPointer<IRecordID> id, const QDateTime &date);
    virtual QSharedPointer<IRecord> getRecordByDate(const QString &id, const QDateTime &date);
    virtual QList<QSharedPointer<IRecord>> getRecordsByDate(const QStringList &ids, const QDateTime& date);
    virtual QStringList getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date);
    virtual QList<QSharedPointer<IRecord> > getHistory(QSharedPointer<IRecordID> recordID);
    virtual QList<QSharedPointer<IRecordID> > getChanges(const QDateTime &fromDate, const QDateTime &toDate);

protected:
    virtual QMap<QString, QSharedPointer<IRecordID> > searchByDate(QSharedPointer<IParameter> parameter, const QDateTime &date);

private:
    QSharedPointer<QObjectLogging> logger;
    QString m_Name;
};

#endif // EDOCTCPHISTORICCLIENT_H
