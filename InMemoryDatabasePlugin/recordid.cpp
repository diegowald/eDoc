#include "recordid.h"

RecordID::RecordID(QObject *parent) :
    QObject(parent)
{
}

RecordID::RecordID(const QString &stringID, QObject *parent) :
    QObject(parent), m_ID(stringID)
{
}

RecordID::~RecordID()
{
}

QString RecordID::asString() const
{
    return m_ID.toString();
}
