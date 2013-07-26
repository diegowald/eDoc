#include "tag.h"

Tag::Tag(QObject *parent) :
    QObject(parent)
{
    keywords = "";
    m_isNull = true;
}

Tag::~Tag()
{
}

void Tag::setValue(const QVariant &newValue)
{
    m_isNull = newValue.isNull();
    keywords = (m_isNull ? "" : newValue.toString());
}

QVariant Tag::asVariant()
{
    if (m_isNull)
        return QVariant(QVariant::String);
    else
        return QVariant(keywords);
}

QVariant Tag::content()
{
    return asVariant();
}

void Tag::setNull()
{
    m_isNull = true;
    keywords = "";
}

bool Tag::isNull()
{
    return m_isNull;
}

QStringList Tag::keys()
{
    QStringList result;
    if (!m_isNull)
        result = keywords.split(" ");
    return result;
}

QString Tag::asString()
{
    if (m_isNull)
        return "";
    else
        return keywords;
}
