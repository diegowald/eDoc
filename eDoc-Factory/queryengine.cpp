#include "queryengine.h"
#include "../eDoc-Configuration/xmlelement.h"

QueryEngine::QueryEngine(QObject *parent) :
    QObject(parent)
{
}

QueryEngine::~QueryEngine()
{
}

void QueryEngine::initialize(XMLCollection *configuration)
{
    QString countStr = ((XMLElement*)configuration->get("count"))->value();
    int count = countStr.toInt();

    for (int i = 1; i <= count; ++i)
    {
        QString queryName = "query%1";
        XMLCollection *queryConf = (XMLCollection*)configuration->get(queryName.arg(i));
        createQueryFromConf(queryConf);
    }
}

void QueryEngine::createQueryFromConf(XMLCollection *configuration)
{
    QString queryName = ((XMLElement*)configuration->get("name"))->value();
    XMLCollection *fieldsConf = (XMLCollection*)configuration->get("fields");
    QString countStr = ((XMLElement*)fieldsConf->get("count"))->value();
    int count = countStr.toInt();

    QStringList fieldNames;
    for (int i = 1; i <= count; ++i)
    {
        QString field = "field%1";
        XMLCollection *fieldDefinition = (XMLCollection*)fieldsConf->get(field.arg(i));

        QString fieldName = ((XMLElement*)fieldDefinition->get("name"))->value();
        fieldNames.push_back(fieldName);
    }
    queries[queryName] = fieldNames;
}

QStringList QueryEngine::getTreeNames() const
{
    return queries.keys();
}

QStringList QueryEngine::getFieldsForTree(const QString &viewName)
{
    if (queries.contains(viewName))
    {
        return queries[viewName];
    }
    else
    {
        return QStringList();
    }
}
