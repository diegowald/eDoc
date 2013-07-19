#include "inmemorydatabase.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-MetadataFramework/fielddefinition.h"
#include "recordid.h"

InMemoryDatabase::InMemoryDatabase(QObject *parent) :
    QObject(parent)
{
}

InMemoryDatabase::~InMemoryDatabase()
{
}

void InMemoryDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
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
    field->initialize(configuration, m_Logger);
    return field;
}

QList<IFieldDefinition*> InMemoryDatabase::fields()
{
    return m_Fields.values();
}

QList<IRecordID*> InMemoryDatabase::search(const QList<IParameter*> &parameters) const
{
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
    return (IRecord*) m_Records[id->asString()];
}

void InMemoryDatabase::updateRecord(IRecord* record)
{
    m_Records[record->ID()->asString()] = (Record*)record;
}

void InMemoryDatabase::deleteRecord(IRecordID *id)
{
    m_Records.remove(id->asString());
}

QString InMemoryDatabase::name()
{
    return "InMemoryDatabase";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(InMemoryDatabasePlugin, InMemoryDatabase)
#endif // QT_VERSION < 0x050000

