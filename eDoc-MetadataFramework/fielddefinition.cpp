#include "fielddefinition.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "valuedefinitions.h"


FieldDefinition::FieldDefinition(QObject *parent) :
    QObject(parent)
{
    m_Name = "";
    m_Type = "";
    m_ReadOnly = "";
    m_Visible = "";
}

FieldDefinition::~FieldDefinition()
{
}

void FieldDefinition::initialize(IXMLContent *configuration, QObjectLogging *logger)
{

    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-MetadataFramework", "void FieldDefinition::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    m_Type = ((XMLElement*)((XMLCollection*) configuration)->get("type"))->value();
    m_ReadOnly = ((XMLElement*)((XMLCollection*) configuration)->get("readonly"))->value() == "1" ? true : false;
    m_Visible = ((XMLElement*)((XMLCollection*) configuration)->get("visible"))->value() == "1" ? true : false;
    m_DataType = analyzeType();
    m_FieldNameInDatabase = ((XMLElement*)((XMLCollection*)configuration)->get("fieldname"))->value();
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
        IDOCBASE_TYPE;
    else if ("document" == tp)
        return IDOCUMENT_TYPE;
    else if ("multidocument" == tp)
        return IMULTIDOCUMENT_TYPE;
    else if ("record" == tp)
        return IRECORD_REFERENCE_TYPE;
    else if ("record" == tp)
        return IMULTIRECORD_REFERENCE_TYPE;
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

bool FieldDefinition::isReadOnly()
{
    return m_ReadOnly;
}

bool FieldDefinition::isVisible()
{
    return m_Visible;
}

QList<VALIDQUERY> FieldDefinition::validQueries()
{
}

IValue* FieldDefinition::createEmptyValue()
{
    IValue *value = NULL;
    switch (m_DataType)
    {
    case INTEGER_TYPE:
        value = new IntegerValue(this);
        break;
    case DOUBLE_TYPE:
        value = new DoubleValue(this);
        break;
    case BOOL_TYPE:
        value = new BoolValue(this);
        break;
    case QSTRING_TYPE:
        value = new QStringValue(this);
        break;
    case QDATETIME_TYPE:
        value = new QDateTimeValue(this);
        break;
    case QDATE_TYPE:
        value = new QDateValue(this);
        break;
    case QTIME_TYPE:
        value = new QTimeValue(this);
        break;
    case IDOCBASE_TYPE:
        value = new IDocBaseValue(NULL, this);
        value->setNull();
        break;
    case IDOCUMENT_TYPE:
        value = new IDocumentValue(NULL, this);
        value->setNull();
        break;
    case IMULTIDOCUMENT_TYPE:
        value = new IMultiDocumentValue(NULL, this);
        value->setNull();
        break;
    case IRECORD_REFERENCE_TYPE:
        value = new IRecordValue(NULL, this);
        value->setNull();
        break;
    case IMULTIRECORD_REFERENCE_TYPE:
        value = new IMultiRecordValue(NULL, this);
        value->setNull();
        break;
    case INVALID_TYPE:
        value = NULL;
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
