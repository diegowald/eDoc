#include "historicdatabase.h"
#include "../eDoc-Configuration/xmlelement.h"
#include <QPluginLoader>
#include <QSet>
#include "../GenericDatabasePlugin/recordid.h"
#include "historicrecord.h"
#include <boost/make_shared.hpp>


HistoricDatabase::HistoricDatabase()
{
}

HistoricDatabase::~HistoricDatabase()
{
}

// IInitializable
void HistoricDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger,
                        const QMap<QString, QString> &docpluginStock,
                        const QMap<QString, QString> &DBplugins, const QMap<QString, QString> &DBWithHistoryPlugins,
                        const QMap<QString, QString> &tagPlugins,
                        const QMap<QString, QString> &serverPlugins)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "HistoricDatabasePlugin", "void HistoricDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    m_MasterTableName = ((XMLElement*)((XMLCollection*)configuration)->get("mastertablename"))->value();
    m_HistoryTableName = ((XMLElement*)((XMLCollection*)configuration)->get("historytablename"))->value();

    m_SQLManager.initialize(configuration, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);

    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("database");
    databaseEngine = createDatabaseEngine(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
}

// IDatabase
QList<IFieldDefinition*> HistoricDatabase::fields()
{
    return databaseEngine->fields();
}

IFieldDefinition* HistoricDatabase::field(const QString &fieldName)
{
    return databaseEngine->field(fieldName);
}

IParameter* HistoricDatabase::createEmptyParameter()
{
    return databaseEngine->createEmptyParameter();
}

QList<IRecordID*> HistoricDatabase::search(const QList<IParameter*> &parameters)
{
    return searchByDate(parameters, now());
}

QList<IRecordID*> HistoricDatabase::searchWithin(const QList<IParameter*> &parameters, const QList<IRecordID*> &records)
{
    QList<IRecordID*> result;
    QStringList recIds;
    QList<IRecordID*> partialResult = search(parameters);
    QStringList partialResultIds;
    QMap<QString, IRecordID*> map;
    foreach (IRecordID* rec, partialResult)
    {
        partialResultIds.append(rec->asString());
    }
    foreach (IRecordID* rec, records)
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


IRecord* HistoricDatabase::createEmptyRecord()
{
    HistoricRecord* record = new HistoricRecord(databaseEngine->createEmptyRecord(), this);
    record->setID(new RecordID(record));
    return record;
}

IRecordID *HistoricDatabase::addRecord(IRecord *record)
{
    HistoricRecord *rec = dynamic_cast<HistoricRecord*>(record);
    if (rec == NULL)
    {
        rec = new HistoricRecord(record, this);
        rec->setID(new RecordID(rec));
    }

    databaseEngine->addRecord(rec->getRecord());

    QString SQLhistory = "INSERT INTO %1 (MasterID, record_id, fromDate) VALUES (:MasterID, :record_id, :fromDate)";
    QString sql = SQLhistory.arg(this->m_HistoryTableName);
    DBRecordPtr r = boost::make_shared<DBRecord>();
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

IRecord* HistoricDatabase::getRecord(IRecordID *id)
{
    return getRecordByDate(id, now());
}

IRecord* HistoricDatabase::getRecord(const QString &id)
{
    return getRecordByDate(id, now());
}

QList<IRecord*> HistoricDatabase::getRecords(const QStringList &ids)
{
    return getRecordsByDate(ids, now());
}

void HistoricDatabase::updateRecord(IRecord* record)
{
    IRecordID *newId = new RecordID(this);
    HistoricRecord *histRec = dynamic_cast<HistoricRecord*>(record);
    if (histRec == NULL)
    {
        histRec = dynamic_cast<HistoricRecord*>(getRecord(record->ID()));
    }
    histRec->getRecord()->setID(newId);

    databaseEngine->addRecord(histRec->getRecord());

    QString SQLhistory = "INSERT INTO %1 (MasterID, record_id, fromDate) VALUES (:MasterID, :record_id, :fromDate)";
    QString sql = SQLhistory.arg(this->m_HistoryTableName);
    DBRecordPtr r = boost::make_shared<DBRecord>();
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

void HistoricDatabase::deleteRecord(IRecordID *id)
{
    QString sqlQuery = "UPDATE %1 SET DeletedDate = :DeletedDate WHERE IDMaster = :MasterID";
    QString sql = sqlQuery.arg(m_MasterTableName);
    DBRecordPtr r = boost::make_shared<DBRecord>();
    (*r)["MasterID"] = id->asString();
    (*r)["DeletedDate"] = now().toMSecsSinceEpoch();
    m_SQLManager.executeCommand(sql, r);
}

QStringList HistoricDatabase::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return databaseEngine->getDistinctColumnValues(filter, columnName);
}

QString HistoricDatabase::name()
{
    return "HistoricDatabase";
}

QMap<QString, IRecordID*> HistoricDatabase::search(IParameter* parameter)
{
    return searchByDate(parameter, now());
}

// IDatabaseWithHistory
QList<IRecordID*> HistoricDatabase::searchByDate(const QList<IParameter*> &parameters, const QDateTime &date)
{
    QMap<QString, IRecordID*> validRecords = getValidRecords(NULL, date);
    QList<IRecordID*> lst = databaseEngine->search(parameters);
    QStringList ids;
    QStringList keys = validRecords.keys();
    foreach (IRecordID* rec, lst)
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

IRecord* HistoricDatabase::getRecordByDate(IRecordID *id, const QDateTime &date)
{
    QMap<QString, IRecordID*> validRecord = getValidRecords(id, date);

    IRecord *record = databaseEngine->getRecord(validRecord.keys().at(0));
    HistoricRecord *rec = new HistoricRecord(record, this);
    rec->setID(id);
    return rec;
}

IRecord* HistoricDatabase::getRecordByDate(const QString &id, const QDateTime &date)
{
    RecordID *recId = new RecordID(id, this);
    return getRecordByDate(recId, date);
}

QList<IRecord*> HistoricDatabase::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    QList<IRecord*> result;

    foreach (QString id, ids)
    {
        result.append(getRecordByDate(id, date));
    }
    return result;
}

QStringList HistoricDatabase::getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date)
{
}

QList<IRecord*> HistoricDatabase::getHistory(IRecordID *recordID)
{
}

QList<IRecordID*> HistoricDatabase::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
}

QMap<QString, IRecordID*> HistoricDatabase::searchByDate(IParameter* parameter, const QDateTime &date)
{
    return QMap<QString, IRecordID*>();
}

QDateTime HistoricDatabase::now() const
{
    return QDateTime::currentDateTimeUtc();
}

IDatabase *HistoricDatabase::createDatabaseEngine(XMLCollection *confEngine,
                                                  const QMap<QString, QString> &docpluginStock,
                                                  const QMap<QString, QString> &DBplugins, const QMap<QString, QString> &DBWithHistoryPlugins,
                                                  const QMap<QString, QString> &tagPlugins,
                                                  const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "database")
    {
        XMLCollection *conf = (XMLCollection*) confEngine;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        if (DBplugins.contains(engineClass)) {
            QPluginLoader pluginLoader(DBplugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IDatabase *engine = qobject_cast<IDatabase*>(plugin);
                engine->initialize(confEngine, m_Logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return qobject_cast<IDatabase *>(plugin);
            }
            else {
                m_Logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            m_Logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return NULL;
}

QMap<QString, IRecordID*> HistoricDatabase::getValidRecords(IRecordID* master_id, const QDateTime &date)
{
    QMap<QString, IRecordID*> result;

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

    DBRecordPtr record = boost::make_shared<DBRecord>();
    (*record)["fromDate"] = date.toMSecsSinceEpoch();
    if (master_id)
    {
        (*record)["MasterId"] = master_id->asString();
    }
    DBRecordSet rs = m_SQLManager.getRecords(sql, record);

    foreach (DBRecordPtr rec, *rs)
    {
        RecordID *mID = new RecordID((*rec)["MasterID"].toString(), this);

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


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(HistoricDatabasePlugin, HistoricDatabase)
#endif // QT_VERSION < 0x050000

