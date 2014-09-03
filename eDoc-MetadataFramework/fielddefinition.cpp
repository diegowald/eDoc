#include "fielddefinition.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "valuedefinitions.h"
#include "tag.h"

FieldDefinition::FieldDefinition(QObject *parent) :
    QObject(parent)
{
    m_Name = "";
    m_Type = "";
    m_ReadOnly = false;
    m_Visible = false;
    m_Queryable = false;
}

FieldDefinition::~FieldDefinition()
{
}

void FieldDefinition::initialize(QSharedPointer<IXMLContent> configuration,
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
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-MetadataFramework", "void FieldDefinition::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();
    m_Type = configuration.dynamicCast<XMLCollection>()->get("type").dynamicCast<XMLElement>()->value();
    m_ReadOnly = configuration.dynamicCast<XMLCollection>()->get("readonly").dynamicCast<XMLElement>()->value() == "1" ? true : false;
    m_Visible = configuration.dynamicCast<XMLCollection>()->get("visible").dynamicCast<XMLElement>()->value() == "1" ? true : false;
    m_DataType = analyzeType();
    m_FieldNameInDatabase = configuration.dynamicCast<XMLCollection>()->get("fieldname").dynamicCast<XMLElement>()->value();
    m_Queryable = true;
    switch (m_DataType)
    {
    case IRECORD_REFERENCE_TYPE:
    case IMULTIRECORD_REFERENCE_TYPE:
        m_OtherDatabaseName = configuration.dynamicCast<XMLCollection>()->get("datanase").dynamicCast<XMLElement>()->value();
        m_FieldToShow = configuration.dynamicCast<XMLCollection>()->get("display_field").dynamicCast<XMLElement>()->value();
        break;
    case IDOCBASE_TYPE:
    case IDOCUMENT_TYPE:
    case IMULTIDOCUMENT_TYPE:
        m_Queryable = false;
        break;
    default:
        break;
    }
/*

    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    persistentEngine = createPersistentEngine(confEngine, pluginStock);

    QList<VALIDQUERY> m_ValidQeries;*/
}

DATATYPE FieldDefinition::analyzeType()
{
    QString tp = m_Type.toLower();
    if ("integer" == tp)
        return INTEGER_TYPE;
    else if ("double" == tp)
        return DOUBLE_TYPE;
    else if ("boolean" == tp)
        return BOOL_TYPE;
    else if ("string" == tp)
        return QSTRING_TYPE;
    else if ("datetime" == tp)
        return QDATETIME_TYPE;
    else if ("date" == tp)
        return QDATE_TYPE;
    else if ("time" == tp)
        return QTIME_TYPE;
    else if ("docbase" == tp)
        return IDOCBASE_TYPE;
    else if ("document" == tp)
        return IDOCUMENT_TYPE;
    else if ("multidocument" == tp)
        return IMULTIDOCUMENT_TYPE;
    else if ("record" == tp)
        return IRECORD_REFERENCE_TYPE;
    else if ("multi_record" == tp)
        return IMULTIRECORD_REFERENCE_TYPE;
    else if ("tag" == tp)
        return TAG_TYPE;
    else
        return INVALID_TYPE;
}

QString FieldDefinition::name()
{
    return m_Name;
}

QString FieldDefinition::type()
{
    return m_Type;
}

bool FieldDefinition::isReadOnly() const
{
    return m_ReadOnly;
}

bool FieldDefinition::isVisible() const
{
    return m_Visible;
}

bool FieldDefinition::isQueryable() const
{
    return m_Queryable;
}

QList<VALIDQUERY> FieldDefinition::validQueries()
{
    return QList<VALIDQUERY>();
}

QSharedPointer<IValue> FieldDefinition::createEmptyValue()
{
    QSharedPointer<IValue> value;
    switch (m_DataType)
    {
    case INTEGER_TYPE:
        value = QSharedPointer<IValue>(new IntegerValue());
        break;
    case DOUBLE_TYPE:
        value = QSharedPointer<IValue>(new DoubleValue());
        break;
    case BOOL_TYPE:
        value = QSharedPointer<IValue>(new BoolValue());
        break;
    case QSTRING_TYPE:
        value = QSharedPointer<IValue>(new QStringValue());
        break;
    case QDATETIME_TYPE:
        value = QSharedPointer<IValue>(new QDateTimeValue());
        break;
    case QDATE_TYPE:
        value = QSharedPointer<IValue>(new QDateValue());
        break;
    case QTIME_TYPE:
        value = QSharedPointer<IValue>(new QTimeValue());
        break;
    case IDOCBASE_TYPE:
        value = QSharedPointer<IValue>(new IDocBaseValue(QSharedPointer<IDocBase>()));
        value->setNull();
        break;
    case IDOCUMENT_TYPE:
        value = QSharedPointer<IValue>(new IDocumentIDValue(NULL));
        value->setNull();
        break;
    case IMULTIDOCUMENT_TYPE:
        value = QSharedPointer<IValue>(new IMultiDocumentValue(QSharedPointer<IMultiDocument>()));
        value->setNull();
        break;
    case IRECORD_REFERENCE_TYPE:
        value = QSharedPointer<IValue>(new IRecordValue(QSharedPointer<IRecord>()));
        value->setNull();
        break;
    case IMULTIRECORD_REFERENCE_TYPE:
        value = QSharedPointer<IValue>(new IMultiRecordValue(QSharedPointer<IMultiRecord>()));
        value->setNull();
        break;
    case TAG_TYPE:
        /*value = new ITagRecordValue(NULL, this);
        QVariant v;
        v.setValue(new Tag());
        value->setValue(v);*/
        value = QSharedPointer<IValue>(new Tag());
        value->setNull();
        break;
    case INVALID_TYPE:
        value = QSharedPointer<IValue>();
        break; // ACA HABRIA QUE LANZAR EXCEPTIONS
    }
    return value;
}

QString FieldDefinition::fieldNameInDatabase()
{
    return m_FieldNameInDatabase;
}

DATATYPE FieldDefinition::dataType()
{
    return m_DataType;
}
