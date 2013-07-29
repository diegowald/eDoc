#ifndef IDATABASE_H
#define IDATABASE_H

#include <QtPlugin>
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

#include "IFieldDefinition.h"
#include "IRecordID.h"
#include "IRecord.h"
#include "IParameter.h"
#include <QSet>


struct IDatabase {
public:
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock) = 0;
    virtual QList<IFieldDefinition*> fields() = 0;
    virtual IFieldDefinition* field(const QString &fieldName) = 0;
    virtual IParameter* createEmptyParameter() = 0;
    virtual QList<IRecordID*> search(const QList<IParameter*> &parameters) = 0;
    virtual IRecord* createEmptyRecord() = 0;
    virtual IRecordID *addRecord(IRecord *record) = 0;
    virtual IRecord* getRecord(IRecordID *id) = 0;
    virtual void updateRecord(IRecord* record) = 0;
    virtual void deleteRecord(IRecordID *id) = 0;
    virtual QString name() = 0;

    virtual ~IDatabase() {}

protected:
    virtual QSet<IRecordID*> search(IParameter* parameter) = 0;
};

Q_DECLARE_INTERFACE(IDatabase, "com.mksingenieria.eDoc.IDatabase/0.0")
#endif // IDATABASE_H
