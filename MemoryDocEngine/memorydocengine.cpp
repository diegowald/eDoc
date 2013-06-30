#include "memorydocengine.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>


MemoryDocEngine::MemoryDocEngine(QObject *parent) :
    QObject(parent)
{
}

MemoryDocEngine::~MemoryDocEngine()
{
}

void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger)
{
}

IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)
{
}

IDocument* MemoryDocEngine::getDocument(IDocID *id) const
{
}

bool MemoryDocEngine::deleteDocument(IDocID *id)
{
}


QString MemoryDocEngine::name()
{
    return "MemoryDocEngine";
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(MemoryDocEngine, MemoryDocEngine)
#endif // QT_VERSION < 0x050000

