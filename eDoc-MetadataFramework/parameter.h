#ifndef PARAMETER_H
#define PARAMETER_H

#include "edoc-metadataframework_global.h"
#include <QObject>
#include "../eDoc-API/IParameter.h"

class EDOCMETADATAFRAMEWORKSHARED_EXPORT Parameter : public QObject, public IParameter
{
    Q_OBJECT
public:
    explicit Parameter(QObject *parent = 0);
    virtual ~Parameter();
    virtual void setField(QSharedPointer<IFieldDefinition> field);
    virtual QSharedPointer<IFieldDefinition> field();
    virtual void addValue(QSharedPointer<IValue> value);
    virtual QList<QSharedPointer<IValue>> values();
    virtual void setQueryType(VALIDQUERY queryType);
    virtual VALIDQUERY queryType();

signals:
    
public slots:
private:
    QSharedPointer<IFieldDefinition> m_Field;
    QList<QSharedPointer<IValue>> m_Values;
    VALIDQUERY m_QueryType;
};

#endif // PARAMETER_H
