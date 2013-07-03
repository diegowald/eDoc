#include "inmemorydatabase.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-MetadataFramework/fielddefinition.h"

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
    m_Logger->logTrace("void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    XMLCollection *confFields = (XMLCollection*)((XMLCollection*)configuration)->get("fields");
    createFields(confFields);
    /*XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    persistentEngine = createPersistentEngine(confEngine, pluginStock);*/
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

QList<IRecord*> InMemoryDatabase::search(const QList<IParameter*> &parameters) const
{
}

QString InMemoryDatabase::name()
{
    return "InMemoryDatabase";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(InMemoryDatabasePlugin, InMemoryDatabase)
#endif // QT_VERSION < 0x050000
