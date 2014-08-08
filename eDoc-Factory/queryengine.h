#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <QObject>
#include "../eDoc-API/IQueryEngine.h"
#include <QMap>
#include <QStringList>

class QueryEngine : public QObject, public IQueryEngine
{
    Q_OBJECT
public:
    explicit QueryEngine(QObject *parent = 0);\
    virtual ~QueryEngine();

    virtual void initialize(XMLCollection *configuration);
    virtual QStringList getTreeNames() const;
    virtual QStringList getFieldsForTree(const QString &viewName);

signals:

public slots:

private:
    void createQueryFromConf(XMLCollection* configuration);

private:
    QMap<QString, QStringList> queries;
};

#endif // QUERYENGINE_H
