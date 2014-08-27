#include "inmemorydatabase.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-MetadataFramework/fielddefinition.h"
#include "recordid.h"
#include "../eDoc-MetadataFramework/parameter.h"

InMemoryDatabase::InMemoryDatabase(QObject *parent) :
    QObject(parent)
{
}

InMemoryDatabase::~InMemoryDatabase()
{
}

void InMemoryDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                  const QMap<QString, QString> &docpluginStock,
                                  const QMap<QString, QString> &DBplugins,
                                  const QMap<QString, QString> &DBWithHistoryPlugins,
                                  const QMap<QString, QString> &tagPlugins,
                                  const QMap<QString, QString> &serverPlugins)
{
    (void)docpluginStock;
    (void)DBplugins;
    (void)DBWithHistoryPlugins;
    (void)tagPlugins;
    (void)serverPlugins;
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "InMemoryDatabasePlugin", "void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    XMLCollection *confFields = (XMLCollection*)((XMLCollection*)configuration)->get("fields");
    createFields(confFields);
}

void InMemoryDatabase::createFields(IXMLContent* configuration)
{
    XMLCollection *confFields = (XMLCollection*)configuration;
    int count = ((XMLElement*)confFields->get("count"))->value().toInt();
    for (int i = 1; i <= count; ++i)
    {
        QString fieldName = "field" + QString::number(i);
        XMLCollection *field = (XMLCollection*)confFields->get(fieldName);
        IFieldDefinition *fDef = createField(field);
        m_Fields[fDef->name()] = fDef;
    }
}

IFieldDefinition *InMemoryDatabase::createField(IXMLContent *configuration)
{
    IFieldDefinition *field = new FieldDefinition(this);
    QMap<QString, QString> empty;
    field->initialize(configuration, m_Logger, empty, empty, empty, empty, empty);
    return field;
}

QList<IFieldDefinition*> InMemoryDatabase::fields()
{
    return m_Fields.values();
}

IFieldDefinition* InMemoryDatabase::field(const QString &fieldName)
{
    return m_Fields.contains(fieldName) ? m_Fields[fieldName] : NULL; //EXCEPTION
}

QList<IRecordID*> InMemoryDatabase::search(const QList<IParameter*> &parameters)
{
}

QList<IRecordID*> InMemoryDatabase::searchWithin(const QList<IParameter*> &parameters, const QList<IRecordID*> &records)
{
}

QMap<QString, IRecordID *> InMemoryDatabase::search(IParameter* parameter)
{
}

IParameter* InMemoryDatabase::createEmptyParameter()
{
    return new Parameter();
}

IRecord* InMemoryDatabase::createEmptyRecord()
{
    QList<IFieldDefinition*> flds = m_Fields.values();
    Record *rec = new Record(flds, this);
    rec->setID(new RecordID(this));
    return rec;
}

IRecordID* InMemoryDatabase::addRecord(IRecord *record)
{
    RecordID *ID = new RecordID(this);
    record->setID(ID);
    m_Records[ID->asString()] = (Record*)record;
    return ID;
}

IRecord* InMemoryDatabase::getRecord(IRecordID *id)
{
    return getRecord(id->asString());
}

IRecord* InMemoryDatabase::getRecord(const QString &id)
{
    return (IRecord*) m_Records[id];
}

void InMemoryDatabase::updateRecord(IRecord* record)
{
    m_Records[record->ID()->asString()] = (Record*)record;
}

QList<IRecord*> InMemoryDatabase::getRecords(const QStringList &ids)
{
    QList<IRecord*> records;
    foreach (QString id, ids)
    {
        records.append((IRecord*) m_Records[id]);
    }
    return records;
}

void InMemoryDatabase::deleteRecord(IRecordID *id)
{
    m_Records.remove(id->asString());
}

QStringList InMemoryDatabase::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    m_Logger->logTrace(__FILE__, __LINE__, "InMemoryDatabase", "QStringList InMemoryDatabase::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName) TODO");
    return QStringList();
}

QString InMemoryDatabase::name()
{
    return "InMemoryDatabase";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(InMemoryDatabasePlugin, InMemoryDatabase)
#endif // QT_VERSION < 0x050000

