#ifndef XMLELEMENT_H
#define XMLELEMENT_H

#include <QObject>
#include "IXMLContent.h"


class EDOCCONFIGURATIONSHARED_EXPORT XMLElement : public QObject, public IXMLContent
{
    Q_OBJECT
public:
    explicit XMLElement(const QString &key, const QString &value, QObject *parent = 0);
    virtual ~XMLElement();
    virtual bool isLeaf() const { return true; }
    virtual QString value() const;
    virtual QString toDebugString(int indentation);
signals:
    
public slots:

private:
    QString m_Value;
};

#endif // XMLELEMENT_H
