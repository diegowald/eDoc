#include "tcpproxyfileid.h"

TCPProxyFileID::TCPProxyFileID(QObject *parent) :
    QObject(parent)
{
}

TCPProxyFileID::TCPProxyFileID(const QString &stringID, QObject *parent) :
    QObject(parent), m_ID(stringID)
{
}

TCPProxyFileID::~TCPProxyFileID()
{
}

QString TCPProxyFileID::asString() const
{
    return m_ID.toString();
}
