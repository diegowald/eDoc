#include "databasewithhistorywrapper.h"

DatabaseWithHistoryWrapper::DatabaseWithHistoryWrapper(QSharedPointer<IDatabase> realDatabase, QSharedPointer<QObjectLogging> Logger, QObject *parent)  :
    QObject(parent)
{
    database = realDatabase;
    logger = Logger;
}

DatabaseWithHistoryWrapper::~DatabaseWithHistoryWrapper()
{
}

// IInitializable
void DatabaseWithHistoryWrapper::initialize(QSharedPointer<IXMLContent> configuration,
                                            QSharedPointer<QObjectLogging> logger,
                                            const QMap<QString, QString> &docpluginStock,
                                            const QMap<QString, QString> &DBPlugins,
                                            const QMap<QString, QString> &DBWithHistoryPlugins,
                                            const QMap<QString, QString> &tagPlugins,
                                            const QMap<QString, QString> &serverPlugins)
{
    (void)configuration;
    (void)logger;
    (void)DBPlugins;
    (void)DBWithHistoryPlugins;
    (void)docpluginStock;
    (void)tagPlugins;
    (void)serverPlugins;
}

// IDatabase
QList<QSharedPointer<IFieldDefinition> > DatabaseWithHistoryWrapper::fields()
{
    return database->fields();
}

QSharedPointer<IFieldDefinition> DatabaseWithHistoryWrapper::field(const QString &fieldName)
{
    return database->field(fieldName);
}

QSharedPointer<IParameter> DatabaseWithHistoryWrapper::createEmptyParameter()
{
    return database->createEmptyParameter();
}

QList<QSharedPointer<IRecordID>> DatabaseWithHistoryWrapper::search(const QList<QSharedPointer<IParameter>> &parameters)
{
    return database->search(parameters);
}

QList<QSharedPointer<IRecordID>> DatabaseWithHistoryWrapper::searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records)
{
    return database->searchWithin(parameters, records);
}

QSharedPointer<IRecord> DatabaseWithHistoryWrapper::createEmptyRecord()
{
    return database->createEmptyRecord();
}

QSharedPointer<IRecordID> DatabaseWithHistoryWrapper::addRecord(QSharedPointer<IRecord> record)
{
    return database->addRecord(record);
}

QSharedPointer<IRecord> DatabaseWithHistoryWrapper::getRecord(QSharedPointer<IRecordID> id)
{
    return database->getRecord(id);
}

QSharedPointer<IRecord> DatabaseWithHistoryWrapper::getRecord(const QString &id)
{
    return database->getRecord(id);
}

QList<QSharedPointer<IRecord> > DatabaseWithHistoryWrapper::getRecords(const QStringList &ids)
{
    return database->getRecords(ids);
}

void DatabaseWithHistoryWrapper::updateRecord(QSharedPointer<IRecord> record)
{
    return database->updateRecord(record);
}

void DatabaseWithHistoryWrapper::deleteRecord(QSharedPointer<IRecordID> id)
{
    return database->deleteRecord(id);
}

QStringList DatabaseWithHistoryWrapper::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return database->getDistinctColumnValues(filter, columnName);
}

QList<QPair<QString, QString>> DatabaseWithHistoryWrapper::getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return database->getColumnValue(filter, columnName);
}

QString DatabaseWithHistoryWrapper::name()
{
    return "DatabaseWithHistoryWrapper";
}

QMap<QString, QSharedPointer<IRecordID> > DatabaseWithHistoryWrapper::search(QSharedPointer<IParameter> parameter)
{
    (void)parameter;
    return QMap<QString, QSharedPointer<IRecordID>>();
}

// IDatabaseWithHistory
QList<QSharedPointer<IRecordID>> DatabaseWithHistoryWrapper::searchByDate(const QList<QSharedPointer<IParameter>> &parameters, const QDateTime &date)
{
    return search(parameters);
}

QSharedPointer<IRecord> DatabaseWithHistoryWrapper::getRecordByDate(QSharedPointer<IRecordID> id, const QDateTime &date)
{
    return getRecord(id);
}

QSharedPointer<IRecord> DatabaseWithHistoryWrapper::getRecordByDate(const QString &id, const QDateTime &date)
{
    return getRecord(id);
}

QList<QSharedPointer<IRecord>> DatabaseWithHistoryWrapper::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    return getRecords(ids);
}

QStringList DatabaseWithHistoryWrapper::getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date)
{
    return getDistinctColumnValues(filter, columnName);
}

QList<QSharedPointer<IRecord>> DatabaseWithHistoryWrapper::getHistory(QSharedPointer<IRecordID> recordID)
{
    return QList<QSharedPointer<IRecord>>();
}

QList<QSharedPointer<IRecordID>> DatabaseWithHistoryWrapper::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
    return QList<QSharedPointer<IRecordID>>();
}

QMap<QString, QSharedPointer<IRecordID>> DatabaseWithHistoryWrapper::searchByDate(QSharedPointer<IParameter> parameter, const QDateTime &date)
{
    return search(parameter);
}
