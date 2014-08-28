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
void HistoricDatabase::initialize(IXMLContent *configuration,
                                  QSharedPointer<QObjectLogging> logger,
                                  const QMap<QString, QString> &docpluginStock,
                                  const QMap<QString, QString> &DBplugins,
                                  const QMap<QString, QString> &DBWithHistoryPlugins,
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
QList<QSharedPointer<IFieldDefinition> > HistoricDatabase::fields()
{
    return databaseEngine->fields();
}

QSharedPointer<IFieldDefinition> HistoricDatabase::field(const QString &fieldName)
{
    return databaseEngine->field(fieldName);
}

QSharedPointer<IParameter> HistoricDatabase::createEmptyParameter()
{
    return databaseEngine->createEmptyParameter();
}

QList<QSharedPointer<IRecordID> > HistoricDatabase::search(const QList<QSharedPointer<IParameter> > &parameters)
{
    return searchByDate(parameters, now());
}

QList<QSharedPointer<IRecordID> > HistoricDatabase::searchWithin(const QList<QSharedPointer<IParameter> > &parameters, const QList<QSharedPointer<IRecordID> > &records)
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


QSharedPointer<IRecord> HistoricDatabase::createEmptyRecord()
{
    QSharedPointer<HistoricRecord> record = QSharedPointer<HistoricRecord>(new HistoricRecord(databaseEngine->createEmptyRecord(), this));
    record->setID(QSharedPointer<IRecordID>(new RecordID(record.data())));
    return record;
}

QSharedPointer<IRecordID> HistoricDatabase::addRecord(QSharedPointer<IRecord> record)
{
    QSharedPointer<HistoricRecord> rec = record.dynamicCast<HistoricRecord>();
    if (rec.isNull())
    {
        rec = QSharedPointer<HistoricRecord>(new HistoricRecord(record, this));
        rec->setID(QSharedPointer<RecordID>(new RecordID(rec.data())));
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
    QSharedPointer<IRecordID> newId = QSharedPointer<IRecordID>(new RecordID(this));
    QSharedPointer<HistoricRecord> histRec =record.dynamicCast<HistoricRecord>();
    if (histRec.isNull())
    {
        histRec = getRecord(record->ID()).dynamicCast<HistoricRecord>();
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

void HistoricDatabase::deleteRecord(QSharedPointer<IRecordID> id)
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
    QMap<QString, QSharedPointer<IRecordID>> validRecord = getValidRecords(id, date);

    QSharedPointer<IRecord> record = databaseEngine->getRecord(validRecord.keys().at(0));
    QSharedPointer<HistoricRecord> rec = QSharedPointer<HistoricRecord>(new HistoricRecord(record, this));
    rec->setID(id);
    return rec;
}

QSharedPointer<IRecord> HistoricDatabase::getRecordByDate(const QString &id, const QDateTime &date)
{
    QSharedPointer<RecordID> recId = QSharedPointer<RecordID>(new RecordID(id, this));
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

QSharedPointer<IDatabase> HistoricDatabase::createDatabaseEngine(XMLCollection *confEngine,
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
                return QSharedPointer<IDatabase>(engine);
            }
            else {
                m_Logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            m_Logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return QSharedPointer<IDatabase>();
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

    DBRecordPtr record = boost::make_shared<DBRecord>();
    (*record)["fromDate"] = date.toMSecsSinceEpoch();
    if (master_id)
    {
        (*record)["MasterId"] = master_id->asString();
    }
    DBRecordSet rs = m_SQLManager.getRecords(sql, record);

    foreach (DBRecordPtr rec, *rs)
    {
        QSharedPointer<IRecordID> mID = QSharedPointer<IRecordID>(new RecordID((*rec)["MasterID"].toString(), this));

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

