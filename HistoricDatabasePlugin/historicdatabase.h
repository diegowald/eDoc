#ifndef HISTORICDATABASEPLUGIN_H
#define HISTORICDATABASEPLUGIN_H

#include "historicdatabaseplugin_global.h"
#include <QObject>
#include <QDateTime>
#include "../eDoc-API/IDatabaseWithHistory.h"
#include "../sqlmanager/sqlmanager.h"
#include "../eDoc-Configuration/xmlcollection.h"

class HISTORICDATABASEPLUGINSHARED_EXPORT HistoricDatabase : public QObject, public IDatabaseWithHistory
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDatabaseWithHistory/0.0" FILE "HistoricDatabasePlugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDatabaseWithHistory)

public:
    HistoricDatabase();
    virtual ~HistoricDatabase();


    // IInitializable
public:
    virtual void initialize(IXMLContent *configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);

    // IDatabase
public:
    virtual QList<QSharedPointer<IFieldDefinition> > fields();
    virtual QSharedPointer<IFieldDefinition> field(const QString &fieldName);
    virtual QSharedPointer<IParameter> createEmptyParameter();
    virtual QList<QSharedPointer<IRecordID> > search(const QList<QSharedPointer<IParameter> > &parameters);
    virtual QList<QSharedPointer<IRecordID> > searchWithin(const QList<QSharedPointer<IParameter> > &parameters, const QList<QSharedPointer<IRecordID> > &records);
    virtual QSharedPointer<IRecord> createEmptyRecord();
    virtual QSharedPointer<IRecordID> addRecord(QSharedPointer<IRecord> record);
    virtual QSharedPointer<IRecord> getRecord(QSharedPointer<IRecordID> id);
    virtual QSharedPointer<IRecord> getRecord(const QString &id);
    virtual QList<QSharedPointer<IRecord>> getRecords(const QStringList &ids);
    virtual void updateRecord(QSharedPointer<IRecord> record);
    virtual void deleteRecord(QSharedPointer<IRecordID> id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();
protected:
    virtual QMap<QString, QSharedPointer<IRecordID> > search(QSharedPointer<IParameter> parameter);

    // IDatabaseWithHistory
public:
    virtual QList<QSharedPointer<IRecordID> > searchByDate(const QList<QSharedPointer<IParameter> > &parameters, const QDateTime &date);
    virtual QSharedPointer<IRecord> getRecordByDate(QSharedPointer<IRecordID> id, const QDateTime &date);
    virtual QSharedPointer<IRecord> getRecordByDate(const QString &id, const QDateTime &date);
    virtual QList<QSharedPointer<IRecord> > getRecordsByDate(const QStringList &ids, const QDateTime& date);
    virtual QStringList getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date);
    virtual QList<QSharedPointer<IRecord> > getHistory(QSharedPointer<IRecordID> recordID);
    virtual QList<QSharedPointer<IRecordID> > getChanges(const QDateTime &fromDate, const QDateTime &toDate);

protected:
    virtual QMap<QString, QSharedPointer<IRecordID> > searchByDate(QSharedPointer<IParameter> parameter, const QDateTime &date);

    // Internal functions
private:
    QDateTime now() const;
    QSharedPointer<IDatabase> createDatabaseEngine(XMLCollection *confEngine,
                                    const QMap<QString, QString> &docpluginStock,
                                    const QMap<QString, QString> &DBplugins,
                                    const QMap<QString, QString> &DBWithHistoryPlugins,
                                    const QMap<QString, QString> &tagPlugins,
                                    const QMap<QString, QString> &serverPlugins);

    QMap<QString, QSharedPointer<IRecordID> > getValidRecords(QSharedPointer<IRecordID> master_id, const QDateTime& date);
    QStringList intersectRecords(QStringList &list1, QStringList &list2);

private:
    QSharedPointer<IDatabase> databaseEngine;
    QSharedPointer<QObjectLogging> m_Logger;
    QString m_Name;
    QString m_MasterTableName;
    QString m_HistoryTableName;
    SQLManager m_SQLManager;

};

#endif // HISTORICDATABASEPLUGIN_H
