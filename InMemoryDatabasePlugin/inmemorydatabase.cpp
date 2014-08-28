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

void InMemoryDatabase::initialize(IXMLContent *configuration,
                                  QSharedPointer<QObjectLogging> logger,
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
        QSharedPointer<IFieldDefinition> fDef = createField(field);
        m_Fields[fDef->name()] = fDef;
    }
}

QSharedPointer<IFieldDefinition> InMemoryDatabase::createField(IXMLContent *configuration)
{
    QSharedPointer<IFieldDefinition> field = QSharedPointer<IFieldDefinition>(new FieldDefinition(this));
    QMap<QString, QString> empty;
    field->initialize(configuration, m_Logger, empty, empty, empty, empty, empty);
    return field;
}

QList<QSharedPointer<IFieldDefinition> > InMemoryDatabase::fields()
{
    return m_Fields.values();
}

QSharedPointer<IFieldDefinition> InMemoryDatabase::field(const QString &fieldName)
{
    return m_Fields.contains(fieldName) ? m_Fields[fieldName] : QSharedPointer<IFieldDefinition>(); //EXCEPTION
}

QList<QSharedPointer<IRecordID>> InMemoryDatabase::search(const QList<QSharedPointer<IParameter>> &parameters)
{
}

QList<QSharedPointer<IRecordID>> InMemoryDatabase::searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records)
{
}

QMap<QString, QSharedPointer<IRecordID>> InMemoryDatabase::search(QSharedPointer<IParameter> parameter)
{
}

QSharedPointer<IParameter> InMemoryDatabase::createEmptyParameter()
{
    return QSharedPointer<IParameter>(new Parameter());
}

QSharedPointer<IRecord> InMemoryDatabase::createEmptyRecord()
{
    QList<QSharedPointer<IFieldDefinition>> flds = m_Fields.values();
    QSharedPointer<IRecord> rec = QSharedPointer<IRecord>(new Record(flds, this));
    rec->setID(QSharedPointer<IRecordID>(new RecordID(this)));
    return rec;
}

QSharedPointer<IRecordID> InMemoryDatabase::addRecord(QSharedPointer<IRecord> record)
{
    QSharedPointer<IRecordID> ID = QSharedPointer<IRecordID>(new RecordID(this));
    record->setID(ID);
    m_Records[ID->asString()] = record;
    return ID;
}

QSharedPointer<IRecord> InMemoryDatabase::getRecord(QSharedPointer<IRecordID> id)
{
    return getRecord(id->asString());
}

QSharedPointer<IRecord> InMemoryDatabase::getRecord(const QString &id)
{
    return m_Records[id];
}

void InMemoryDatabase::updateRecord(QSharedPointer<IRecord> record)
{
    m_Records[record->ID()->asString()] = record;
}

QList<QSharedPointer<IRecord> > InMemoryDatabase::getRecords(const QStringList &ids)
{
    QList<QSharedPointer<IRecord>> records;
    foreach (QString id, ids)
    {
        records.append(m_Records[id]);
    }
    return records;
}

void InMemoryDatabase::deleteRecord(QSharedPointer<IRecordID> id)
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

