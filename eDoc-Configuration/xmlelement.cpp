#include "xmlelement.h"

XMLElement::XMLElement(const QString &key, const QString &value, QObject *parent) :
    QObject(parent)
{
    this->key(key);
    m_Value = value;
}

XMLElement::~XMLElement()
{
}

QString XMLElement::value() const
{
    return m_Value;
}

QString XMLElement::toDebugString(int indentation)
{
    QString s;
    QString ind(indentation, ' ');
    s += ind + key() + "= " + value() + "\n";
    return s;
}
