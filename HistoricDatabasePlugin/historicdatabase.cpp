#include "historicdatabase.h"
#include "../eDoc-API/IFactory.h"
#include "../eDoc-Configuration/xmlelement.h"
#include <QPluginLoader>
#include <QSet>
#include "../GenericDatabasePlugin/recordid.h"
#include "historicrecord.h"


HistoricDatabase::HistoricDatabase() : QObject(), m_SQLManager(this)
{
}

HistoricDatabase::~HistoricDatabase()
{
    databaseEngine.clear();
}

// IInitializable

void HistoricDatabase::initialize(IXMLContentPtr configuration, IFactory* factory)
{
    m_Logger = factory->logger();
    m_Logger->logTrace(__FILE__, __LINE__, "HistoricDatabasePlugin", "void HistoricDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();

    m_MasterTableName = configuration.dynamicCast<XMLCollection>()->get("mastertablename").dynamicCast<XMLElement>()->value();
    m_HistoryTableName = configuration.dynamicCast<XMLCollection>()->get("historytablename").dynamicCast<XMLElement>()->value();

    m_SQLManager.initialize(configuration, factory);

    QSharedPointer<XMLCollection> confEngine = configuration.dynamicCast<XMLCollection>()->get("database").dynamicCast<XMLCollection>();
    databaseEngine = factory->createDatabase(confEngine);
}

// IDatabase
QList<IFieldDefinitionPtr> HistoricDatabase::fields()
{
    return databaseEngine->fields();
}

IFieldDefinitionPtr HistoricDatabase::field(const QString &fieldName)
{
    return databaseEngine->field(fieldName);
}

IParameterPtr HistoricDatabase::createEmptyParameter()
{
    return databaseEngine->createEmptyParameter();
}

QList<IRecordIDPtr> HistoricDatabase::search(const QList<IParameterPtr> &parameters)
{
    return searchByDate(parameters, now());
}

QList<IRecordIDPtr> HistoricDatabase::searchWithin(const QList<IParameterPtr> &parameters, const QList<IRecordIDPtr> &records)
{
    QList<QSharedPointer<IRecordID>> result;
    QStringList recIds;
    QList<QSharedPointer<IRecordID>> partialResult = search(parameters);
    QStringList partialResultIds;
    QMap<QString, QSharedPointer<IRecordID>> map;
    foreach (QSharedPointer<IRecordID> rec, partialResult)
    {
        partialResultIds.append(rec->asString());
    }
    foreach (QSharedPointer<IRecordID> rec, records)
    {
        recIds.append(rec->asString());
        map[rec->asString()] = rec;
    }
    QStringList resultIds = intersectRecords(partialResultIds, recIds);
    foreach (QString id, resultIds)
    {
        result.append(map[id]);
    }
    return result;
}


IRecordPtr HistoricDatabase::createEmptyRecord()
{
    QSharedPointer<HistoricRecord> record = QSharedPointer<HistoricRecord>(new HistoricRecord(databaseEngine->createEmptyRecord()));
    record->setID(QSharedPointer<IRecordID>(new RecordID(record.data())));
    return record;
}

IRecordIDPtr HistoricDatabase::addRecord(IRecordPtr record)
{
    QSharedPointer<HistoricRecord> rec = record.dynamicCast<HistoricRecord>();
    if (rec.isNull())
    {
        rec = QSharedPointer<HistoricRecord>(new HistoricRecord(record));
        rec->setID(QSharedPointer<RecordID>(new RecordID(record->ID()->asString())));
    }
    rec->getRecord()->setID(QSharedPointer<RecordID>(new RecordID()));
    databaseEngine->addRecord(rec->getRecord());

    QString SQLhistory = "INSERT INTO %1 (MasterID, record_id, fromDate) VALUES (:MasterID, :record_id, :fromDate)";
    QString sql = SQLhistory.arg(m_HistoryTableName);
    DBRecordPtr r = DBRecordPtr(new DBRecord());
    (*r)["MasterID"] = rec->ID()->asString();
    (*r)["record_id"] = rec->getRecord()->ID()->asString();
    (*r)["fromDate"] = now().toMSecsSinceEpoch();
    int currentId = m_SQLManager.executeCommandAndReturnId(sql, r);

    QString SQLMaster = "INSERT INTO %1 (IDMaster, CurrentRecord) VALUES (:IDMaster, :currentRecord)";
    sql = SQLMaster.arg(m_MasterTableName);
    (*r).clear();
    (*r)["IDMaster"] = rec->ID()->asString();
    (*r)["currentRecord"] = currentId;
    m_SQLManager.executeCommand(sql, r);

    return rec->ID();
}

QSharedPointer<IRecord> HistoricDatabase::getRecord(QSharedPointer<IRecordID> id)
{
    return getRecordByDate(id, now());
}

QSharedPointer<IRecord> HistoricDatabase::getRecord(const QString &id)
{
    return getRecordByDate(id, now());
}

QList<QSharedPointer<IRecord> > HistoricDatabase::getRecords(const QStringList &ids)
{
    return getRecordsByDate(ids, now());
}

void HistoricDatabase::updateRecord(QSharedPointer<IRecord> record)
{
    QSharedPointer<IRecordID> newId = QSharedPointer<IRecordID>(new RecordID());
    QSharedPointer<HistoricRecord> histRec =record.dynamicCast<HistoricRecord>();
    if (histRec.isNull())
    {
        histRec = getRecord(record->ID()).dynamicCast<HistoricRecord>();
    }
    histRec->getRecord()->setID(newId);

    databaseEngine->addRecord(histRec->getRecord());

    QString SQLhistory = "INSERT INTO %1 (MasterID, record_id, fromDate) VALUES (:MasterID, :record_id, :fromDate)";
    QString sql = SQLhistory.arg(m_HistoryTableName);
    DBRecordPtr r = DBRecordPtr(new DBRecord());
    (*r)["MasterID"] = histRec->ID()->asString();
    (*r)["record_id"] = histRec->getRecord()->ID()->asString();
    (*r)["fromDate"] = now().toMSecsSinceEpoch();
    int currentId = m_SQLManager.executeCommandAndReturnId(sql, r);

    QString SQLMaster = "UPDATE %1 SET currentRecord = :currentRecord WHERE IDMaster = :IDMaster";
    sql = SQLMaster.arg(m_MasterTableName);
    (*r).clear();
    (*r)["IDMaster"] = histRec->ID()->asString();
    (*r)["currentRecord"] = currentId;
    m_SQLManager.executeCommand(sql, r);
}

void HistoricDatabase::deleteRecord(QSharedPointer<IRecordID> id)
{
    QString sqlQuery = "UPDATE %1 SET DeletedDate = :DeletedDate WHERE IDMaster = :MasterID";
    QString sql = sqlQuery.arg(m_MasterTableName);
    DBRecordPtr r = DBRecordPtr(new DBRecord());
    (*r)["MasterID"] = id->asString();
    (*r)["DeletedDate"] = now().toMSecsSinceEpoch();
    m_SQLManager.executeCommand(sql, r);
}

QStringList HistoricDatabase::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return getDistinctColumnValuesByDate(filter, columnName, now());
}

QList<QPair<QString, QString> > HistoricDatabase::getColumnValue(const QList<QPair<QString, QString> > &filter, const QString &columnName)
{
    return databaseEngine->getColumnValue(filter, columnName);
}

QString HistoricDatabase::name()
{
    return "HistoricDatabase";
}

QMap<QString, QSharedPointer<IRecordID> > HistoricDatabase::search(QSharedPointer<IParameter> parameter)
{
    return searchByDate(parameter, now());
}

// IDatabaseWithHistory
QList<QSharedPointer<IRecordID> > HistoricDatabase::searchByDate(const QList<QSharedPointer<IParameter> > &parameters, const QDateTime &date)
{
    QMap<QString, QSharedPointer<IRecordID>> validRecords = getValidRecords(QSharedPointer<IRecordID>(), date);
    QList<QSharedPointer<IRecordID>> lst = databaseEngine->search(parameters);
    QStringList ids;
    QStringList keys = validRecords.keys();
    foreach (QSharedPointer<IRecordID> rec, lst)
    {
        ids.append(rec->asString());
    }

    QStringList result = intersectRecords(keys, ids);

    lst.clear();
    foreach (QString rec, result)
    {
        lst.append(validRecords[rec]);
    }

    return lst;
}

QSharedPointer<IRecord> HistoricDatabase::getRecordByDate(QSharedPointer<IRecordID> id, const QDateTime &date)
{
    HistoricRecordPtr rec = HistoricRecordPtr();

    QMap<QString, QSharedPointer<IRecordID>> validRecord = getValidRecords(id, date);
    if (validRecord.count() > 0)
    {
        IRecordPtr record = databaseEngine->getRecord(validRecord.keys().at(0));
        rec = HistoricRecordPtr(new HistoricRecord(record));
        rec->setID(id);
    }
    return rec;
}

QSharedPointer<IRecord> HistoricDatabase::getRecordByDate(const QString &id, const QDateTime &date)
{
    QSharedPointer<RecordID> recId = QSharedPointer<RecordID>(new RecordID(id));
    return getRecordByDate(recId, date);
}

QList<QSharedPointer<IRecord> > HistoricDatabase::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    QList<QSharedPointer<IRecord>> result;

    foreach (QString id, ids)
    {
        result.append(getRecordByDate(id, date));
    }
    return result;
}

QStringList HistoricDatabase::getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date)
{
    QList<QPair<QString, QString>> result = databaseEngine->getColumnValue(filter, columnName);
    QMap<QString, QSharedPointer<IRecordID> > validRecords = getValidRecords(QSharedPointer<IRecordID>(), date);

    QSet<QString> distinctValues;
    QPair<QString, QString> pair;
    foreach (pair, result)
    {
        if (validRecords.contains(pair.first))
            distinctValues.insert(pair.second);
    }

    return distinctValues.toList();
}

QList<QSharedPointer<IRecord> > HistoricDatabase::getHistory(QSharedPointer<IRecordID> recordID)
{
}

QList<QSharedPointer<IRecordID> > HistoricDatabase::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
}

QMap<QString, QSharedPointer<IRecordID> > HistoricDatabase::searchByDate(QSharedPointer<IParameter> parameter, const QDateTime &date)
{
    return QMap<QString, QSharedPointer<IRecordID>>();
}

QDateTime HistoricDatabase::now() const
{
    return QDateTime::currentDateTimeUtc();
}

QMap<QString, QSharedPointer<IRecordID> > HistoricDatabase::getValidRecords(QSharedPointer<IRecordID> master_id, const QDateTime &date)
{
    QMap<QString, QSharedPointer<IRecordID>> result;

    QString sqlQuery;
    if (master_id)
    {
        sqlQuery = "SELECT MasterId, record_id, max(id) from %1 group by masterId having fromdate < :fromDate and MasterId = :MasterId";
    }
    else
    {
        sqlQuery = "SELECT MasterId, record_id, max(id) from %1 group by masterId having fromdate < :fromDate";
    }


    QString sql = sqlQuery.arg(m_HistoryTableName);

    DBRecordPtr record = DBRecordPtr(new DBRecord());
    (*record)["fromDate"] = date.toMSecsSinceEpoch();
    if (master_id)
    {
        (*record)["MasterId"] = master_id->asString();
    }
    DBRecordSet rs = m_SQLManager.getRecords(sql, record);

    foreach (DBRecordPtr rec, *rs)
    {
        QSharedPointer<IRecordID> mID = QSharedPointer<IRecordID>(new RecordID((*rec)["MasterID"].toString()));

        result[(*rec)["record_id"].toString()] = mID;
    }
    return result;
}

QStringList HistoricDatabase::intersectRecords(QStringList &list1, QStringList &list2)
{
    QSet<QString> set1(QSet<QString>::fromList(list1));
    QSet<QString> set2(QSet<QString>::fromList(list2));

    return set1.intersect(set2).toList();
}

IDatabasePtr HistoricDatabase::newDatabase()
{
    return IDatabasePtr(new HistoricDatabase());
}

IDatabaseWithHistoryPtr HistoricDatabase::newDatabaseWithHistory()
{
    return IDatabaseWithHistoryPtr(new HistoricDatabase());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(HistoricDatabasePlugin, HistoricDatabase)
#endif // QT_VERSION < 0x050000

