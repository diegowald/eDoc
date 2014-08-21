#include "docengine.h"
#include <QtWidgets>

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

DocEngine::~DocEngine()
{
}

void DocEngine::initialize(IXMLContent *configuration, QObjectLogging *logger,
                           const QMap<QString, QString> &docpluginStock,
                           const QMap<QString, QString> &DBplugins,
                           const QMap<QString, QString> &tagPlugins,
                           const QMap<QString, QString> &serverPlugins)
{
    (void)configuration;
    (void)logger;
    (void)docpluginStock;
    (void)DBplugins;
    (void)tagPlugins;
    (void)serverPlugins;
}

QString DocEngine::name()
{
    return "DocEngine";
}

IDocID* DocEngine::addDocument(const QByteArray& blob)
{
    DocID *id = DocID::createNew();
    Document *doc = new Document(id);
    collection[*id] = doc;
    return id;
}

IDocument* DocEngine::getDocument(IDocID *id)
{
    DocID *iid = (DocID*)(id);
    return collection[*iid];
}

bool DocEngine::deleteDocument(IDocID *id)
{
    collection.remove(*(DocID*)id);
    return true;
}

IDocID* DocEngine::IValueToIDocId(IValue *value)
{
    return new DocID(value->content().toString());
}

//Q_EXPORT_PLUGIN2(edoccore, DocEngine);
