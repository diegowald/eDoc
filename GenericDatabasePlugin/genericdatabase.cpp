#include "genericdatabase.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-MetadataFramework/fielddefinition.h"
#include "../eDoc-MetadataFramework/record.h"
#include "recordid.h"
#include <QStringList>
#include <boost/make_shared.hpp>

GenericDatabase::GenericDatabase(QObject *parent) :
    QObject(parent)
{
}

GenericDatabase::~GenericDatabase()
{
}

void GenericDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    m_Logger = logger;
    m_Logger->logTrace("void GenericDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    XMLCollection *confFields = (XMLCollection*)((XMLCollection*)configuration)->get("fields");
    createFields(confFields);

    m_TableName = ((XMLElement*)((XMLCollection*)configuration)->get("tablename"))->value();
    m_SQLManager.initialize(configuration, logger, pluginStock);
}

void GenericDatabase::createFields(IXMLContent* configuration)
{
    XMLCollection *confFields = (XMLCollection*)configuration;
    int count = ((XMLElement*)confFields->get("count"))->value().toInt();
    for (int i = 1; i <= count; ++i)
    {
        QString fieldName = "field" + QString::number(i);
        XMLCollection *field = (XMLCollection*)confFields->get(fieldName);
        IFieldDefinition *fDef = createField(field);
        m_Fields[fDef->name()] = fDef;
        m_FieldsBasedOnDatabase[((FieldDefinition*)fDef)->fieldNameInDatabase()] = fDef;
    }
}

IFieldDefinition *GenericDatabase::createField(IXMLContent *configuration)
{
    IFieldDefinition *field = new FieldDefinition(this);
    field->initialize(configuration, m_Logger);
    return field;
}

QList<IFieldDefinition*> GenericDatabase::fields()
{
    return m_Fields.values();
}

QList<IRecordID*> GenericDatabase::search(const QList<IParameter*> &parameters) const
{
}

IRecord* GenericDatabase::createEmptyRecord()
{
    Record *rec = new Record(m_Fields.values(), this);
    rec->setID(new RecordID(this));
    return rec;
}

IRecordID *GenericDatabase::addRecord(IRecord *record)
{
    QString SQLInsert = "INSERT INTO %1 (%2) VALUES (%3);";
    QString sql = SQLInsert.arg(m_TableName)
            .arg(getFieldsString()).arg(getParametersString());\
    executeSQLCommand(sql, record);
    return record->ID();
}

IRecord* GenericDatabase::getRecord(IRecordID *id)
{
    QString SQLSelect = "SELECT %1 FROM %2 WHERE %3 = %4";
    QString sql = SQLSelect.arg(getFieldsString())
            .arg(m_TableName).arg("record_id").arg(":record_id");

    DBRecordPtr r = boost::make_shared<DBRecord>();
    (*r)["record_id"] = id->asString();
    DBRecordSet res = m_SQLManager.getRecords(sql, r);
    if (0 == res->count())
        return NULL;

    DBRecordPtr recPtr = res->at(0);

    IRecord *record = new Record(m_Fields.values(), this);

    foreach (QString key, recPtr->keys())
    {
        record->value(key)->setValue(((*recPtr)[key]).toString());
    }
    return record;
}

void GenericDatabase::updateRecord(IRecord* record)
{
    QString SQLUpdate = "UPDATE %1 SET %2 WHERE %3 = %4;";
    QString sql = SQLUpdate.arg(m_TableName).arg(getUpdateFieldsString()).arg("record_id").arg(":record_id");
    executeSQLCommand(sql, record);
}

void GenericDatabase::executeSQLCommand(const QString &sql, IRecord* record)
{
    DBRecordPtr r = boost::make_shared<DBRecord>();
//    (*r)["record_id"] = id->asString();
    (*r)["record_id"] = record->ID()->asString();
    foreach (QString key, m_Fields.keys())
    {
        if (record->value(key)->isNull())
            (*r)[((FieldDefinition*)m_Fields[key])->fieldNameInDatabase()] = QVariant(QVariant::String);
        else
            (*r)[((FieldDefinition*)m_Fields[key])->fieldNameInDatabase()] = record->value(key)->asString();
    }

    m_SQLManager.executeCommand(sql, r);
}

void GenericDatabase::deleteRecord(IRecordID *id)
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
    fields.append(m_Fields.keys());
    QStringList parameters;
    foreach(QString field, fields)
        parameters.append(":" + field);
    return parameters.join(", ");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GenericDatabasePlugin, GenericDatabase)
#endif // QT_VERSION < 0x050000

