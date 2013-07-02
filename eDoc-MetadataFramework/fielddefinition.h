#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <QObject>
#include "../eDoc-API/IFieldDefinition.h"
#include "../eDoc-Configuration/qobjectlgging.h"

class FieldDefinition : public QObject, public IFieldDefinition
{
    Q_OBJECT
public:
    explicit FieldDefinition(QObject *parent = 0);
    virtual ~FieldDefinition();
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock) = 0;
    virtual QString name();
    virtual QString type();
    virtual bool isReadOnly();
    virtual bool isVisible();
    virtual QList<VALIDQUERY> validQueries();
signals:
    
public slots:
private:
    QString m_Name;
    QString m_Type;
    bool m_ReadOnly;
    bool m_Visible;
    QList<VALIDQUERY> m_ValidQeries;
    QObjectLogging *m_Logger;
};

#endif // FIELDDEFINITION_H
