#include "simplefileid.h"

SimpleFileID::SimpleFileID(QObject *parent) :
    QObject(parent)
{
    m_ID = QUuid::createUuid();
}

SimpleFileID::SimpleFileID(const QString &stringID, QObject *parent) :
    QObject(parent), m_ID(stringID)
{
}

SimpleFileID::~SimpleFileID()
{
}

QString SimpleFileID::asString() const
{
    return m_ID.toString();
}
