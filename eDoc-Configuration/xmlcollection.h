#ifndef XMLCOLLECTION_H
#define XMLCOLLECTION_H

#include <QObject>
#include "IXMLContent.h"
#include <QMap>


class EDOCCONFIGURATIONSHARED_EXPORT XMLCollection : public QObject, public IXMLContent
{
    Q_OBJECT
public:
    explicit XMLCollection(QObject *parent = 0);
    virtual ~XMLCollection();
    virtual bool isLeaf() const { return false; }
    virtual void addXML(IXMLContent *content);
    virtual IXMLContent* get(const QString &key);
    virtual QString toDebugString(int indentation);
signals:
    
public slots:
    
private:
    QMap<QString, IXMLContent*> m_Content;
};

#endif // XMLCOLLECTION_H
