#include "databasewithhistorywrapper.h"

DatabaseWithHistoryWrapper::DatabaseWithHistoryWrapper(IDatabase* realDatabase, QObjectLogging* Logger, QObject *parent)  :
    QObject(parent)
{
    database = realDatabase;
    logger = Logger;
}

DatabaseWithHistoryWrapper::~DatabaseWithHistoryWrapper()
{
}

// IInitializable
void DatabaseWithHistoryWrapper::initialize(IXMLContent *configuration, QObjectLogging *logger,
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
QList<IFieldDefinition*> DatabaseWithHistoryWrapper::fields()
{
    return database->fields();
}

IFieldDefinition* DatabaseWithHistoryWrapper::field(const QString &fieldName)
{
    return database->field(fieldName);
}

IParameter* DatabaseWithHistoryWrapper::createEmptyParameter()
{
    return database->createEmptyParameter();
}

QList<IRecordID*> DatabaseWithHistoryWrapper::search(const QList<IParameter*> &parameters)
{
    return database->search(parameters);
}

QList<IRecordID*> DatabaseWithHistoryWrapper::searchWithin(const QList<IParameter*> &parameters, const QList<IRecordID*> &records)
{
    return database->searchWithin(parameters, records);
}

IRecord* DatabaseWithHistoryWrapper::createEmptyRecord()
{
    return database->createEmptyRecord();
}

IRecordID *DatabaseWithHistoryWrapper::addRecord(IRecord *record)
{
    return database->addRecord(record);
}

IRecord* DatabaseWithHistoryWrapper::getRecord(IRecordID *id)
{
    return database->getRecord(id);
}

IRecord* DatabaseWithHistoryWrapper::getRecord(const QString &id)
{
    return database->getRecord(id);
}

QList<IRecord*> DatabaseWithHistoryWrapper::getRecords(const QStringList &ids)
{
    return database->getRecords(ids);
}

void DatabaseWithHistoryWrapper::updateRecord(IRecord* record)
{
    return database->updateRecord(record);
}

void DatabaseWithHistoryWrapper::deleteRecord(IRecordID *id)
{
    return database->deleteRecord(id);
}

QStringList DatabaseWithHistoryWrapper::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return database->getDistinctColumnValues(filter, columnName);
}

QString DatabaseWithHistoryWrapper::name()
{
    return "DatabaseWithHistoryWrapper";
}

QMap<QString, IRecordID*> DatabaseWithHistoryWrapper::search(IParameter* parameter)
{
    (void)parameter;
    return QMap<QString, IRecordID*>();
}

// IDatabaseWithHistory
QList<IRecordID*> DatabaseWithHistoryWrapper::searchByDate(const QList<IParameter*> &parameters, const QDateTime &date)
{
    return search(parameters);
}

IRecord* DatabaseWithHistoryWrapper::getRecordByDate(IRecordID *id, const QDateTime &date)
{
    return getRecord(id);
}

IRecord* DatabaseWithHistoryWrapper::getRecordByDate(const QString &id, const QDateTime &date)
{
    return getRecord(id);
}

QList<IRecord*> DatabaseWithHistoryWrapper::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    return getRecords(ids);
}

QStringList DatabaseWithHistoryWrapper::getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date)
{
    return getDistinctColumnValues(filter, columnName);
}

QList<IRecord*> DatabaseWithHistoryWrapper::getHistory(IRecordID *recordID)
{
    return QList<IRecord*>();
}

QList<IRecordID*> DatabaseWithHistoryWrapper::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
    return QList<IRecordID*>();
}

QMap<QString, IRecordID*> DatabaseWithHistoryWrapper::searchByDate(IParameter* parameter, const QDateTime &date)
{
    return search(parameter);
}
