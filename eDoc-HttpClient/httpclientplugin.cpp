#include "httpclientplugin.h"

ver estos links
http://doc.qt.digia.com/solutions/4/qtservice/qtservice-example-server.html
http://qt-project.org/doc/qt-5.0/qtnetwork/fortuneclient-client-cpp.html

HttpClientPlugin::HttpClientPlugin(QObject *parent) :
    QObject(parent)
{
}

HttpClientPlugin::~HttpClientPlugin()
{
}

void HttpClientPlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
}

IDocID* HttpClientPlugin::addDocument(const QByteArray& blob)
{
}

IDocBase* HttpClientPlugin::getDocument(IDocID *id)
{
}

bool HttpClientPlugin::deleteDocument(IDocID *id)
{
}

QString HttpClientPlugin::name()
{
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-HttpClient, HttpClientPlugin)
#endif // QT_VERSION < 0x050000
