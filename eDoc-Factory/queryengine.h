#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <QObject>
#include "../eDoc-API/IQueryEngine.h"
#include <QMap>
#include <QStringList>
#include "../eDoc-Configuration/xmlcollection.h"

class QueryEngine : public QObject, public IQueryEngine
{
    Q_OBJECT
public:
    explicit QueryEngine(QObject *parent = 0);\
    virtual ~QueryEngine();

    virtual void initialize(QSharedPointer<IXMLContent> configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins);
    virtual QStringList getTreeNames() const;
    virtual QStringList getFieldsForTree(const QString &viewName);

signals:

public slots:

private:
    void createQueryFromConf(QSharedPointer<XMLCollection> configuration);

private:
    QMap<QString, QStringList> queries;
};

#endif // QUERYENGINE_H
