#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include "../eDoc-API/IRecord.h"
#include "recordid.h"
#include <QMap>
#include "../eDoc-API/IFieldDefinition.h"

class Record : public QObject, public IRecord
{
    Q_OBJECT
public:
    explicit Record(const QList<IFieldDefinition*> &fieldDefs, QObject *parent = 0);
    virtual ~Record();

    virtual void setID(IRecordID *ID);
    virtual IRecordID *ID();
    virtual IValue* value(IFieldDefinition* field);
    virtual IValue* value(const QString &fieldName);
signals:
    
public slots:

private:
    RecordID *m_ID;
    QMap<QString, IValue*> m_Values;
};

#endif // RECORD_H
