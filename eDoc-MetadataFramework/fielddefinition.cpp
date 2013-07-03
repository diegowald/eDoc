#include "fielddefinition.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"

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
    m_Logger->logTrace("void FieldDefinition::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();
    m_Type = ((XMLElement*)((XMLCollection*) configuration)->get("type"))->value();
    m_ReadOnly = ((XMLElement*)((XMLCollection*) configuration)->get("readonly"))->value() == "1" ? true : false;
    m_Visible = ((XMLElement*)((XMLCollection*) configuration)->get("visible"))->value() == "1" ? true : false;

/*

    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    persistentEngine = createPersistentEngine(confEngine, pluginStock);

    QList<VALIDQUERY> m_ValidQeries;*/
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
