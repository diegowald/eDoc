#include "fielddefinition.h"
#include <QPluginLoader>
#include "../eDoc-API/IFactory.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "valuedefinitions.h"
#include "tag.h"
#include <QJsonArray>

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

void FieldDefinition::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    m_Logger = factory->logger();

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
    {
        m_Queryable = false;
        IXMLContentPtr confEngine = configuration.dynamicCast<XMLCollection>()->get("engine");
        engine = factory->createEngine(confEngine);
        break;
    }
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

IValuePtr FieldDefinition::createEmptyValue()
{
    IValuePtr value;
    switch (m_DataType)
    {
    case INTEGER_TYPE:
        value = IValuePtr(new IntegerValue());
        break;
    case DOUBLE_TYPE:
        value = IValuePtr(new DoubleValue());
        break;
    case BOOL_TYPE:
        value = IValuePtr(new BoolValue());
        break;
    case QSTRING_TYPE:
        value = IValuePtr(new QStringValue());
        break;
    case QDATETIME_TYPE:
        value = IValuePtr(new QDateTimeValue());
        break;
    case QDATE_TYPE:
        value = IValuePtr(new QDateValue());
        break;
    case QTIME_TYPE:
        value = IValuePtr(new QTimeValue());
        break;
    case IDOCBASE_TYPE:
        value = IValuePtr(new IDocBaseValue(IDocBasePtr(), this));
        value->setNull();
        break;
    case IDOCUMENT_TYPE:
        value = IValuePtr(new IDocumentValue(IDocumentPtr(), this));
        value->setNull();
        break;
    case IMULTIDOCUMENT_TYPE:
        value = IValuePtr(new IMultiDocumentValue(IMultiDocumentPtr(), this));
        value->setNull();
        break;
    case IRECORD_REFERENCE_TYPE:
        value = IValuePtr(new IRecordValue(IRecordPtr()));
        value->setNull();
        break;
    case IMULTIRECORD_REFERENCE_TYPE:
        value = IValuePtr(new IMultiRecordValue(IMultiRecordPtr()));
        value->setNull();
        break;
    case TAG_TYPE:
        /*value = new ITagRecordValue(NULL, this);
        QVariant v;
        v.setValue(new Tag());
        value->setValue(v);*/
        value = IValuePtr(new Tag());
        value->setNull();
        break;
    case INVALID_TYPE:
        value = IValuePtr();
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

IDocEnginePtr FieldDefinition::getEngine()
{
    return engine;
}

QJsonObject FieldDefinition::asJson()
{
    QJsonObject obj;
    obj["name"] = name();
    obj["type"] = type();
    obj["isReadOnly"] = isReadOnly();
    obj["isVisible"] = isVisible();
    obj["isQueryable"] = isQueryable();

    obj["count"] = validQueries().count();
    QJsonArray arr;
    foreach (VALIDQUERY query, validQueries())
    {
        QJsonObject o;
        o["query"] = query;
        arr.append(o);
    }
    obj["validQueries"] = arr;
    return obj;
}
