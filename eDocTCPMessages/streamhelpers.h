#ifndef STREAMHELPERS_H
#define STREAMHELPERS_H

#include <QDataStream>
#include "../eDoc-API/IParameter.h"
#include "../eDoc-API/IRecord.h"
#include "../eDoc-API/IRecordID.h"
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"

#include "proxyparameter.h"
#include "proxyrecord.h"
#include "proxyrecordid.h"
#include "proxyvalue.h"
#include "proxyfielddefinition.h"

#include "../eDoc-API/IDocID.h"
#include "proxydocid.h"
#include "edoctcpmessages_global.h"

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IParameter &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyParameter &obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IRecord &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyRecord &obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IRecordID &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyRecordID &obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IValue &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyValue &obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IFieldDefinition &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyFieldDefinition &obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IDocID &obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyDocID &obj);

#endif // STREAMHELPERS_H

