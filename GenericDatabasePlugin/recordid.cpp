#include "recordid.h"

RecordID::RecordID(QObject *parent) :
    QObject(parent), m_ID(QUuid::createUuid())
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

QSharedPointer<IMetadata> RecordID::metadata()
{
    return QSharedPointer<IMetadata>();
}
