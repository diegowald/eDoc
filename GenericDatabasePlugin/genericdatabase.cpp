#include "genericdatabase.h"

GenericDatabase::GenericDatabase(QObject *parent) :
    QObject(parent)
{
}

GenericDatabase::~GenericDatabase()
{
}

void GenericDatabase::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
}

QList<IFieldDefinition*> GenericDatabase::fields()
{
}

QList<IRecordID*> GenericDatabase::search(const QList<IParameter*> &parameters) const
{
}

IRecord* GenericDatabase::createEmptyRecord()
{
}

IRecordID *GenericDatabase::addRecord(IRecord *record)
{
}

IRecord* GenericDatabase::getRecord(IRecordID *id)
{
}

void GenericDatabase::updateRecord(IRecord* record)
{
}

void GenericDatabase::deleteRecord(IRecordID *id)
{
}

QString GenericDatabase::name()
{
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GenericDatabasePlugin, GenericDatabase)
#endif // QT_VERSION < 0x050000

