#ifndef IJSONSERIALIZABLE_H
#define IJSONSERIALIZABLE_H

#include <QJsonObject>

struct IJsonSerializable
{
    virtual QJsonObject asJson() = 0;
};

#endif // IJSONSERIALIZABLE_H
