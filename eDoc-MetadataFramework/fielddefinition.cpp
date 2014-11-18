#include "fielddefinition.h"
#include <QPluginLoader>
#include "../eDoc-API/IFactory.h"
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
    case DATATYPE::IRECORD_REFERENCE_TYPE:
    case DATATYPE::IMULTIRECORD_REFERENCE_TYPE:
        m_OtherDatabaseName = configuration.dynamicCast<XMLCollection>()->get("datanase").dynamicCast<XMLElement>()->value();
        m_FieldToShow = configuration.dynamicCast<XMLCollection>()->get("display_field").dynamicCast<XMLElement>()->value();
        break;
    case DATATYPE::IDOCBASE_TYPE:
    case DATATYPE::IDOCUMENT_TYPE:
    case DATATYPE::IMULTIDOCUMENT_TYPE:
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
        return DATATYPE::INTEGER_TYPE;
    else if ("double" == tp)
        return DATATYPE::DOUBLE_TYPE;
    else if ("boolean" == tp)
        return DATATYPE::BOOL_TYPE;
    else if ("string" == tp)
        return DATATYPE::QSTRING_TYPE;
    else if ("datetime" == tp)
        return DATATYPE::QDATETIME_TYPE;
    else if ("date" == tp)
        return DATATYPE::QDATE_TYPE;
    else if ("time" == tp)
        return DATATYPE::QTIME_TYPE;
    else if ("docbase" == tp)
        return DATATYPE::IDOCBASE_TYPE;
    else if ("document" == tp)
        return DATATYPE::IDOCUMENT_TYPE;
    else if ("multidocument" == tp)
        return DATATYPE::IMULTIDOCUMENT_TYPE;
    else if ("record" == tp)
        return DATATYPE::IRECORD_REFERENCE_TYPE;
    else if ("multi_record" == tp)
        return DATATYPE::IMULTIRECORD_REFERENCE_TYPE;
    else if ("tag" == tp)
        return DATATYPE::TAG_TYPE;
    else
        return DATATYPE::INVALID_TYPE;
}

QString FieldDefinition::name()
{
    return m_Name;
}

DATATYPE FieldDefinition::type()
{
    return analyzeType();
}

QString FieldDefinition::typeAsString()
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
    case DATATYPE::INTEGER_TYPE:
        value = IValuePtr(new IntegerValue());
        break;
    case DATATYPE::DOUBLE_TYPE:
        value = IValuePtr(new DoubleValue());
        break;
    case DATATYPE::BOOL_TYPE:
        value = IValuePtr(new BoolValue());
        break;
    case DATATYPE::QSTRING_TYPE:
        value = IValuePtr(new QStringValue());
        break;
    case DATATYPE::QDATETIME_TYPE:
        value = IValuePtr(new QDateTimeValue());
        break;
    case DATATYPE::QDATE_TYPE:
        value = IValuePtr(new QDateValue());
        break;
    case DATATYPE::QTIME_TYPE:
        value = IValuePtr(new QTimeValue());
        break;
    case DATATYPE::IDOCBASE_TYPE:
        value = IValuePtr(new IDocBaseValue(IDocBasePtr(), this));
        value->setNull();
        break;
    case DATATYPE::IDOCUMENT_TYPE:
        value = IValuePtr(new IDocumentValue(IDocumentPtr(), this));
        value->setNull();
        break;
    case DATATYPE::IMULTIDOCUMENT_TYPE:
        value = IValuePtr(new IMultiDocumentValue(IMultiDocumentPtr(), this));
        value->setNull();
        break;
    case DATATYPE::IRECORD_REFERENCE_TYPE:
        value = IValuePtr(new IRecordValue(IRecordPtr()));
        value->setNull();
        break;
    case DATATYPE::IMULTIRECORD_REFERENCE_TYPE:
        value = IValuePtr(new IMultiRecordValue(IMultiRecordPtr()));
        value->setNull();
        break;
    case DATATYPE::TAG_TYPE:
        /*value = new ITagRecordValue(NULL, this);
        QVariant v;
        v.setValue(new Tag());
        value->setValue(v);*/
        value = IValuePtr(new Tag());
        value->setNull();
        break;
    case DATATYPE::INVALID_TYPE:
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

