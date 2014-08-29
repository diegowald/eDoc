#include "genericdatabase.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-MetadataFramework/fielddefinition.h"
#include "../eDoc-MetadataFramework/record.h"
#include "recordid.h"
#include <QStringList>
#include <boost/make_shared.hpp>
#include "../eDoc-InMemoryTagging/inmemorytagprocessor.h"
#include <QSet>
#include "../eDoc-MetadataFramework/parameter.h"
#include <QPair>
#include <QList>

GenericDatabase::GenericDatabase(QObject *parent) :
    QObject(parent), m_SQLManager(this)
{
}

GenericDatabase::~GenericDatabase()
{
}

void GenericDatabase::initialize(IXMLContent *configuration,
                                 QSharedPointer<QObjectLogging> logger,
                                 const QMap<QString, QString> &docpluginStock,
                                 const QMap<QString, QString> &DBplugins,
                                 const QMap<QString, QString> &DBWithHistoryPlugins,
                                 const QMap<QString, QString> &tagPlugins,
                                 const QMap<QString, QString> &serverPlugins)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void GenericDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    XMLCollection *confFields = (XMLCollection*)((XMLCollection*)configuration)->get("fields");
    createFields(confFields);

    m_TableName = ((XMLElement*)((XMLCollection*)configuration)->get("tablename"))->value();
    m_SQLManager.initialize(configuration, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
}

void GenericDatabase::createFields(IXMLContent* configuration)
{
    m_Logger->logTrace(__FILE__, __LINE__, "GenericDatabasePlugin", "void GenericDatabase::createFields(IXMLContent* configuration)");
    XMLCollection *confFields = (XMLCollection*)configuration;
    int count = ((XMLElement*)confFields->get("count"))->value().toInt();
    for (int i = 1; i <= count; ++i)
    {
        QString fieldName = "field" + QString::number(i);
        XMLCollection *field = (XMLCollection*)confFields->get(fieldName);
        QSharedPointer<IFieldDefinition> fDef = createField(field);
        m_Fields[fDef->name()] = fDef;
        m_FieldsBasedOnDatabase[fDef.dynamicCast<FieldDefinition>()->fieldNameInDatabase()] = fDef;
    }
}

QSharedPointer<IFieldDefinition> GenericDatabase::createField(IXMLContent *configuration)
{
    QSharedPointer<IFieldDefinition> field = QSharedPointer<IFieldDefinition>(new FieldDefinition(this));
    QMap<QString, QString> empty;
    field->initialize(configuration, m_Logger, empty, empty, empty, empty, empty);
    return field;
}

QList<QSharedPointer<IFieldDefinition> > GenericDatabase::fields()
{
    return m_Fields.values();
}

QSharedPointer<IFieldDefinition> GenericDatabase::field(const QString &fieldName)
{
    return m_Fields.contains(fieldName) ? m_Fields[fieldName] : QSharedPointer<IFieldDefinition>();
}

QMap<QString, QSharedPointer<IRecordID>> GenericDatabase::intersect(const QMap<QString, QSharedPointer<IRecordID>> &set1, const QMap<QString, QSharedPointer<IRecordID>> &set2)
{
    QMap<QString, QSharedPointer<IRecordID>> result;

    if ((set1.count() == 0) || (set2.count() == 0))
        return result;

    QSet<QString> res = set1.keys().toSet().intersect(set2.keys().toSet());
    foreach (QString key, res)
    {
        result[key] = set1[key];
    }
    return result;
}

QList<QSharedPointer<IRecordID> > GenericDatabase::search(const QList<QSharedPointer<IParameter>> &parameters)
{
    QList<QSharedPointer<IRecordID>> resultList;
    QMap<QString, QSharedPointer<IRecordID>> result;
    bool firstParameterProcessed = false;
    // La idea es ir obteniendo sets para cada parametro y luego realizar la interseccion entre todos.
    foreach (QSharedPointer<IParameter> p, parameters)
    {
        QMap<QString, QSharedPointer<IRecordID>> partialResult = search(p);

        if (partialResult.size() == 0)
            return resultList;

        if (firstParameterProcessed)
            result = intersect(result, partialResult);
        else
        {
            result = partialResult;
            firstParameterProcessed = true;
        }
    }
    return result.values();
}

QList<QSharedPointer<IRecordID>> GenericDatabase::searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records)
{
    QMap<QString, QSharedPointer<IRecordID>> recs;
    foreach (QSharedPointer<IRecordID> rec, records)
    {
        recs[rec->asString()] = rec;
    }

    QList<QSharedPointer<IRecordID>> searchResult = search(parameters);
    QMap<QString, QSharedPointer<IRecordID>> searchRecs;
    foreach (QSharedPointer<IRecordID> rec, searchResult)
    {
        searchRecs[rec->asString()] = rec;
    }

    return intersect(recs, searchRecs).values();
}

std::pair<QString, DBRecordPtr> GenericDatabase::getWhereClause(QSharedPointer<IParameter> parameter)
{
    QString whereClause = "";
    DBRecordPtr r = boost::make_shared<DBRecord>();

    switch (parameter->queryType())
    {
    case EQUALS_TO:
        whereClause = QString("%1 = :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case DISTINT_TO:
        whereClause = QString("%1 <> :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case LESS_THAN:
        whereClause = QString("%1 < :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case LESS_THAN_OR_EQUALS_TO:
        whereClause = QString("%1 <= :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case GREATER_THAN:
        whereClause = QString("%1 > :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case GREATER_THAN_OR_EQUALS_TO:
        whereClause = QString("%1 >= :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        break;
    case BETWEEN:
        whereClause = QString("%1 BETWEEN :%2 AND :%3TO").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant();
        (*r)[QString("%1TO").arg(parameter->field()->name())] = parameter->values().at(1)->asVariant();
        break;
    case CONTAINS:
        whereClause = QString("%1 LIKE :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = "%" + parameter->values().at(0)->asVariant().toString() + "%";
        break;
    case STARTS_WITH:
        whereClause = QString("%1 LIKE :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = parameter->values().at(0)->asVariant().toString() + "%";
        break;
    case ENDS_WITH:
        whereClause = QString("%1 LIKE :%2").arg(parameter->field()->name()).arg(parameter->field()->name());
        (*r)[QString("%1").arg(parameter->field()->name())] = "%" + parameter->values().at(0)->asVariant().toString();
        break;
    case IS_NULL:
        whereClause = QString("(%1 IS NULL OR %1 = '')").arg(parameter->field()->name());
        break;
    case IS_NOT_NULL:
        whereClause = QString("(%1 IS NOT NULL AND %1 <> '')").arg(parameter->field()->name());
        break;
    }


    return std::pair<QString, DBRecordPtr>(whereClause, r);
}

QMap<QString, QSharedPointer<IRecordID> > GenericDatabase::search(QSharedPointer<IParameter> parameter)
{
    QString SQL = "SELECT record_id from %1 where %2";
    std::pair<QString, DBRecordPtr> whereClause = getWhereClause(parameter);
    QString sql = SQL.arg(m_TableName).arg(whereClause.first);
    DBRecordSet rs = m_SQLManager.getRecords(sql, whereClause.second);

    QMap<QString, QSharedPointer<IRecordID>> result;
    foreach (DBRecordPtr rec, *rs)
    {
        QSharedPointer<IRecordID> rID = QSharedPointer<IRecordID>(new RecordID((*rec)["record_id"].toString(), this));
        result[rID->asString()] = rID;
    }
    return result;
}

QSharedPointer<IParameter> GenericDatabase::createEmptyParameter()
{
    return QSharedPointer<IParameter>(new Parameter(this));
}

QSharedPointer<IRecord> GenericDatabase::createEmptyRecord()
{
    QSharedPointer<IRecord> rec = QSharedPointer<IRecord>(new Record(m_Fields.values(), this));
    rec->setID(QSharedPointer<IRecordID>(new RecordID(this)));
    return rec;
}

QSharedPointer<IRecordID> GenericDatabase::addRecord(QSharedPointer<IRecord> record)
{
    QString SQLInsert = "INSERT INTO %1 (%2) VALUES (%3);";
    QString sql = SQLInsert.arg(m_TableName)
            .arg(getFieldsString()).arg(getParametersString());\
    executeSQLCommand(sql, record);
    return record->ID();
}

QSharedPointer<IRecord> GenericDatabase::getRecord(QSharedPointer<IRecordID> id)
{
    return getRecord(id->asString());
}

QSharedPointer<IRecord> GenericDatabase::getRecord(const QString &id)
{
    QString SQLSelect = "SELECT %1 FROM %2 WHERE %3 = %4";
    QString sql = SQLSelect.arg(getFieldsString())
            .arg(m_TableName).arg("record_id").arg(":record_id");

    DBRecordPtr r = boost::make_shared<DBRecord>();
    (*r)["record_id"] = id;
    DBRecordSet res = m_SQLManager.getRecords(sql, r);
    if (0 == res->count())
        return QSharedPointer<IRecord>();

    DBRecordPtr recPtr = res->at(0);

    QSharedPointer<IRecord> record = QSharedPointer<IRecord>(new Record(m_Fields.values(), this));

    foreach (QString key, recPtr->keys())
    {
        m_Logger->logDebug(key);

        if ("record_id" == key)
        {
            record->setID(QSharedPointer<IRecordID>(new RecordID(((*recPtr)[key]).toString(), this)));
        }
        else
        {
            m_Logger->logDebug(m_FieldsBasedOnDatabase[key]->name());
            m_Logger->logDebug(((*recPtr)[key]).toString());
            if (!((*recPtr)[key]).isNull())
                record->value(m_FieldsBasedOnDatabase[key]->name())->setValue(((*recPtr)[key]).toString());
            else
                record->value(m_FieldsBasedOnDatabase[key]->name())->setNull();
        }
    }
    return record;
}

QList<QSharedPointer<IRecord> > GenericDatabase::getRecords(const QStringList &ids)
{
    QList<QSharedPointer<IRecord>> records;
    foreach (QString id, ids)
    {
        records.append(getRecord(id));
    }
    return records;
}

void GenericDatabase::updateRecord(QSharedPointer<IRecord> record)
{
    QString SQLUpdate = "UPDATE %1 SET %2 WHERE %3 = %4;";
    QString sql = SQLUpdate.arg(m_TableName).arg(getUpdateFieldsString()).arg("record_id").arg(":record_id");
    executeSQLCommand(sql, record);
}

void GenericDatabase::executeSQLCommand(const QString &sql, QSharedPointer<IRecord> record)
{
    DBRecordPtr r = boost::make_shared<DBRecord>();
//    (*r)["record_id"] = id->asString();
    (*r)["record_id"] = record->ID()->asString();
    foreach (QString key, m_Fields.keys())
    {
        (*r)[m_Fields[key].dynamicCast<FieldDefinition>()->fieldNameInDatabase()] = record->value(key)->content();
    }

    m_SQLManager.executeCommand(sql, r);
}

void GenericDatabase::deleteRecord(QSharedPointer<IRecordID> id)
{
    QString SQLDelete = "DELETE %1 WHERE %2 = %3";

    QString sql = SQLDelete.arg(m_TableName).arg("record_id").arg(":record_id");

    DBRecordPtr r = boost::make_shared<DBRecord>();
    (*r)["record_id"] = id->asString();
    m_SQLManager.executeCommand(sql, r);
}

QString GenericDatabase::name()
{
    return "GenericDatabase";
}


QString GenericDatabase::getFieldsString()
{
    QStringList fields;
    fields.append("record_id");
    fields.append(m_FieldsBasedOnDatabase.keys());//  m_Fields.keys();

    return fields.join(", ");
}

QString GenericDatabase::getUpdateFieldsString()
{
    QStringList fields;
    fields.append("record_id");
    fields.append(m_FieldsBasedOnDatabase.keys());
    QStringList res;
    foreach (QString field, fields) {
        res.push_back(QString("%1 = :%2").arg(field).arg(field));
    }
    return res.join(", ");
}

QString GenericDatabase::getParametersString()
{
    QStringList fields;
    fields.append("record_id");
    fields.append(m_FieldsBasedOnDatabase.keys());
    QStringList parameters;
    foreach(QString field, fields)
        parameters.append(":" + field);
    return parameters.join(", ");
}

QStringList GenericDatabase::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    QString sql = "SELECT DISTINCT %1 FROM %2 WHERE %1 <> '' AND %1 IS NOT NULL %3";
    QString whereClause = "";
    if (filter.count() > 0)
    {
        QString field;
        QPair<QString, QString> column;
        foreach (column, filter)
        {
            field = "%1 = :%2";
            whereClause += whereClause.length() > 0 ? " AND " : "";
            whereClause += field.arg(column.first, column.first);
        }
        whereClause = " AND " + whereClause;
    }
    QString sqlToExecute = sql.arg(columnName).arg(m_TableName).arg(whereClause);

    QStringList result = m_SQLManager.getDistintValues(sqlToExecute, filter);

    return result;
}

QList<QPair<QString, QString>> GenericDatabase::getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    QString sql = "SELECT record_id, %1 FROM %2 WHERE %1 <> '' AND %1 IS NOT NULL %3";
    QString whereClause = "";
    if (filter.count() > 0)
    {
        QString field;
        QPair<QString, QString> column;
        foreach (column, filter)
        {
            field = "%1 = :%2";
            whereClause += whereClause.length() > 0 ? " AND " : "";
            whereClause += field.arg(column.first, column.first);
        }
        whereClause = " AND " + whereClause;
    }
    QString sqlToExecute = sql.arg(columnName).arg(m_TableName).arg(whereClause);

    QList<QPair<QString, QString>> result = m_SQLManager.getColumnValues(sqlToExecute, filter);

    return result;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GenericDatabasePlugin, GenericDatabase)
#endif // QT_VERSION < 0x050000

