#ifndef IINITIALIZABLE_H
#define IINITIALIZABLE_H

#include <QSharedPointer>
#include <QMap>
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

struct IInitializable {
public:
    virtual void initialize(IXMLContentPtr configuration,
                            QObjectLoggingPtr logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBPlugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins) = 0;


    virtual ~IInitializable() {}

};

#endif // IINITIALIZABLE_H
