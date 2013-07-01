#ifndef IDATABASE_H
#define IDATABASE_H
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

#include "IFieldDefinition.h"
#include "IRecord.h"
#include "IParameter.h"

struct IDatabase {
public:
    virtual void initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock) = 0;
    virtual QList<IFieldDefinition*> fields() = 0;
    virtual QList<IRecord*> search(const QList<IParameter*> &parameters) const = 0;
    virtual QString name() = 0;
    virtual ~IDatabase() {}
};

#endif // IDATABASE_H
