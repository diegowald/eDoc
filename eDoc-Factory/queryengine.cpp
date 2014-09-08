#include "queryengine.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"

QueryEngine::QueryEngine(QObject *parent) :
    QObject(parent)
{
}

QueryEngine::~QueryEngine()
{
}

void QueryEngine::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    (void) factory;

    QString countStr = configuration.dynamicCast<XMLCollection>()->get("count").dynamicCast<XMLElement>()->value();
    int count = countStr.toInt();

    for (int i = 1; i <= count; ++i)
    {
        QString queryName = "query%1";
        QSharedPointer<XMLCollection> queryConf = configuration.dynamicCast<XMLCollection>()->get(queryName.arg(i)).dynamicCast<XMLCollection>();
        createQueryFromConf(queryConf);
    }
}

void QueryEngine::createQueryFromConf(QSharedPointer<XMLCollection> configuration)
{
    QString queryName = configuration->get("name").dynamicCast<XMLElement>()->value();
    QSharedPointer<XMLCollection> fieldsConf = configuration->get("fields").dynamicCast<XMLCollection>();
    QString countStr = fieldsConf->get("count").dynamicCast<XMLElement>()->value();
    int count = countStr.toInt();

    QStringList fieldNames;
    for (int i = 1; i <= count; ++i)
    {
        QString field = "field%1";
        QSharedPointer<XMLCollection> fieldDefinition = fieldsConf->get(field.arg(i)).dynamicCast<XMLCollection>();

        QString fieldName = fieldDefinition->get("name").dynamicCast<XMLElement>()->value();
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
