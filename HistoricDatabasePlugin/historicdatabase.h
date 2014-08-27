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
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);

    // IDatabase
public:
    virtual QList<IFieldDefinition*> fields();
    virtual IFieldDefinition* field(const QString &fieldName);
    virtual IParameter* createEmptyParameter();
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters);
    virtual QList<IRecordID*> searchWithin(const QList<IParameter*> &parameters, const QList<IRecordID*> &records);
    virtual IRecord* createEmptyRecord();
    virtual IRecordID *addRecord(IRecord *record);
    virtual IRecord* getRecord(IRecordID *id);
    virtual IRecord* getRecord(const QString &id);
    virtual QList<IRecord*> getRecords(const QStringList &ids);
    virtual void updateRecord(IRecord* record);
    virtual void deleteRecord(IRecordID *id);
    virtual QStringList getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName);
    virtual QString name();
protected:
    virtual QMap<QString, IRecordID*> search(IParameter* parameter);

    // IDatabaseWithHistory
public:
    virtual QList<IRecordID*> searchByDate(const QList<IParameter*> &parameters, const QDateTime &date);
    virtual IRecord* getRecordByDate(IRecordID *id, const QDateTime &date);
    virtual IRecord* getRecordByDate(const QString &id, const QDateTime &date);
    virtual QList<IRecord*> getRecordsByDate(const QStringList &ids, const QDateTime& date);
    virtual QStringList getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date);
    virtual QList<IRecord*> getHistory(IRecordID *recordID);
    virtual QList<IRecordID*> getChanges(const QDateTime &fromDate, const QDateTime &toDate);

protected:
    virtual QMap<QString, IRecordID*> searchByDate(IParameter* parameter, const QDateTime &date);

    // Internal functions
private:
    QDateTime now() const;
    IDatabase *createDatabaseEngine(XMLCollection *confEngine,
                                    const QMap<QString, QString> &docpluginStock,
                                    const QMap<QString, QString> &DBplugins,
                                    const QMap<QString, QString> &DBWithHistoryPlugins,
                                    const QMap<QString, QString> &tagPlugins,
                                    const QMap<QString, QString> &serverPlugins);

    QMap<QString, IRecordID *> getValidRecords(IRecordID *master_id, const QDateTime& date);
    QStringList intersectRecords(QStringList &list1, QStringList &list2);

private:
    IDatabase *databaseEngine;
    QObjectLogging *m_Logger;
    QString m_Name;
    QString m_MasterTableName;
    QString m_HistoryTableName;
    SQLManager m_SQLManager;

};

#endif // HISTORICDATABASEPLUGIN_H