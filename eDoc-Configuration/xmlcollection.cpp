#include "xmlcollection.h"

XMLCollection::XMLCollection(QObject *parent) :
    QObject(parent)
{
}

XMLCollection::~XMLCollection()
{
    m_Content.clear();
}

void XMLCollection::addXML(QSharedPointer<IXMLContent> content)
{
    m_Content[content->key()] = content;
}

IXMLContentPtr XMLCollection::get(const QString &key)
{
    if (m_Content.contains(key))
        return m_Content[key];
    else
        return QSharedPointer<IXMLContent>();
}

QString XMLCollection::toDebugString(int indentation)
{
    QString s;
    QString ind(indentation, ' ');
    s += ind + key() + " {\n";
    IXMLContentPtr content;
    foreach (content, m_Content.values()) {
        s += content->toDebugString(indentation + 2);
    }
    s += ind + "}\n";
    return s;
}
