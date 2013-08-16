#ifndef HTTPCLIENTPLUGIN_H
#define HTTPCLIENTPLUGIN_H

#include <IDocEngine.h>
#include <QMap>
#include <../eDoc-Configuration/xmlcollection.h>

class HttpClientPlugin : public QObject, public IDocEngine
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.mksingenieria.eDoc.IDocEngine/0.0" FILE "eDoc-HttpClient.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(IDocEngine)
public:
    HttpClientPlugin(QObject *parent = 0);

    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock);
    virtual IDocID* addDocument(const QByteArray& blob);
    virtual IDocBase* getDocument(IDocID *id);
    virtual bool deleteDocument(IDocID *id);
    virtual QString name();
    virtual ~HttpClientPlugin();
};

#endif // HTTPCLIENTPLUGIN_H

