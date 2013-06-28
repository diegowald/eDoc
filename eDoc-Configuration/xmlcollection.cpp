#include "xmlcollection.h"

XMLCollection::XMLCollection(QObject *parent) :
    QObject(parent)
{
}

XMLCollection::~XMLCollection()
{
}

void XMLCollection::addXML(IXMLContent *content)
{
    m_Content[content->key()] = content;
}

IXMLContent * XMLCollection::get(const QString &key)
{
    if (m_Content.contains(key))
        return m_Content[key];
    else
        return NULL;
}

QString XMLCollection::toDebugString(int indentation)
{
    QString s;
    QString ind(indentation, ' ');
    s += ind + key() + " {\n";
    foreach (IXMLContent* content, m_Content.values()) {
        s += content->toDebugString(indentation + 2);
    }
    s += ind + "}";
    return s;
}
