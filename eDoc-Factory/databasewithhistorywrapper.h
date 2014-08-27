#ifndef DATABASEWITHHISTORYWRAPPER_H
#define DATABASEWITHHISTORYWRAPPER_H

#include <QObject>
#include "../eDoc-API/IDatabaseWithHistory.h"

class DatabaseWithHistoryWrapper : public QObject, public IDatabaseWithHistory
{
    Q_OBJECT
public:
    explicit DatabaseWithHistoryWrapper(IDatabase* realDatabase, QObjectLogging* Logger, QObject *parent = 0);
    virtual ~DatabaseWithHistoryWrapper();

    // IInitializable
public:
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBPlugins,
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

signals:

public slots:

private:
    IDatabase *database;
    QObjectLogging *logger;
};

#endif // DATABASEWITHHISTORYWRAPPER_H
