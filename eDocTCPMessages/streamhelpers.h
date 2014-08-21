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

QDataStream& operator<<(QDataStream& os, IParameter &obj);
QDataStream& operator>>(QDataStream& is, ProxyParameter &obj);

QDataStream& operator<<(QDataStream& os, IRecord &obj);
QDataStream& operator>>(QDataStream& is, ProxyRecord &obj);

QDataStream& operator<<(QDataStream& os, IRecordID &obj);
QDataStream& operator>>(QDataStream& is, ProxyRecordID &obj);

QDataStream& operator<<(QDataStream& os, IValue &obj);
QDataStream& operator>>(QDataStream& is, ProxyValue &obj);

QDataStream& operator<<(QDataStream& os, IFieldDefinition &obj);
QDataStream& operator>>(QDataStream& is, ProxyFieldDefinition &obj);

#endif // STREAMHELPERS_H

