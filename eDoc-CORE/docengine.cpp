#include "docengine.h"
#include <QtWidgets>

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

DocEngine::~DocEngine()
{
}

void DocEngine::initialize(QSharedPointer<IXMLContent> configuration,
                           QSharedPointer<QObjectLogging> logger,
                           const QMap<QString, QString> &docpluginStock,
                           const QMap<QString, QString> &DBplugins,
                           const QMap<QString, QString> &DBWithHistoryPlugins,
                           const QMap<QString, QString> &tagPlugins,
                           const QMap<QString, QString> &serverPlugins)
{
    (void)configuration;
    (void)logger;
    (void)docpluginStock;
    (void)DBplugins;
    (void)DBWithHistoryPlugins;
    (void)tagPlugins;
    (void)serverPlugins;
}

QString DocEngine::name()
{
    return "DocEngine";
}

QSharedPointer<IDocID> DocEngine::addDocument(const QByteArray& blob)
{
    QSharedPointer<DocID> id = QSharedPointer<DocID>(DocID::createNew());
    QSharedPointer<Document> doc = QSharedPointer<Document>(new Document(id));
    collection[*id] = doc;
    return id;
}

QSharedPointer<IDocBase> DocEngine::getDocument(QSharedPointer<IDocID> id)
{
    QSharedPointer<DocID> iid = id.dynamicCast<DocID>();
    return collection[*iid];
}

bool DocEngine::deleteDocument(QSharedPointer<IDocID> id)
{
    QSharedPointer<DocID> iid = id.dynamicCast<DocID>();
    collection.remove(*iid);
    return true;
}

QSharedPointer<IDocID> DocEngine::IValueToIDocId(QSharedPointer<IValue> value)
{
    return QSharedPointer<IDocID>(new DocID(value->content().toString()));
}

//Q_EXPORT_PLUGIN2(edoccore, DocEngine);
