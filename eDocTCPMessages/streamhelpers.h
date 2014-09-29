#ifndef STREAMHELPERS_H
#define STREAMHELPERS_H

#include <QDataStream>
#include "../eDoc-API/IParameter.h"
#include "../eDoc-API/IRecord.h"
#include "../eDoc-API/IRecordID.h"
#include "../eDoc-API/IValue.h"
#include "../eDoc-API/IFieldDefinition.h"
#include "../eDoc-API/IDocBase.h"

#include "proxyparameter.h"
#include "proxyrecord.h"
#include "proxyrecordid.h"
#include "proxydocumentvalue.h"
#include "proxyvalue.h"
#include "proxyfielddefinition.h"
#include "../eDoc-MetadataFramework/valuedefinitions.h"
#include "proxydocumentvalue.h"

#include "../eDoc-API/IDocID.h"
#include "proxydocid.h"
#include "edoctcpmessages_global.h"

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IParameterPtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyParameterPtr obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IRecordPtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyRecordPtr obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IRecordIDPtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyRecordIDPtr obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, QSharedPointer<IDocumentValue> obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& os, ProxyDocumentValuePtr obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IValuePtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyValuePtr obj);


EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IFieldDefinitionPtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyFieldDefinitionPtr obj);

EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& os, IDocIDPtr obj);
EDOCTCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& is, ProxyDocIDPtr obj);

#endif // STREAMHELPERS_H

