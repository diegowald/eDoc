#ifndef ISERVER_H
#define ISERVER_H

#include <QtPlugin>
#include "IInitializable.h"

struct IServer;
typedef QSharedPointer<IServer> IServerPtr;

struct IServer : public IInitializable {
public:
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual QString name() const = 0;
    virtual IServerPtr newServer() = 0;

    virtual ~IServer() {}

};

Q_DECLARE_INTERFACE(IServer, "com.mksingenieria.eDoc.IServer/0.0")

#endif // ISERVER_H
