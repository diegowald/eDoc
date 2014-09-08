#include "docengine.h"
#include <QtWidgets>

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

DocEngine::~DocEngine()
{
}

void DocEngine::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    (void)configuration;
    (void)factory;
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

IDocEnginePtr DocEngine::newDocEngine()
{
    return IDocEnginePtr(new DocEngine());
}

//Q_EXPORT_PLUGIN2(edoccore, DocEngine);
