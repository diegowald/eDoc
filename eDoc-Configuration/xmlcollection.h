#ifndef XMLCOLLECTION_H
#define XMLCOLLECTION_H

#include <QObject>
#include <QSharedPointer>
#include "IXMLContent.h"
#include <QMap>


class EDOCCONFIGURATIONSHARED_EXPORT XMLCollection : public QObject, public IXMLContent
{
    Q_OBJECT
public:
    explicit XMLCollection(QObject *parent = 0);
    virtual ~XMLCollection();
    virtual bool isLeaf() const { return false; }
    virtual void addXML(IXMLContentPtr content);
    virtual IXMLContentPtr get(const QString &key);
    virtual QString toDebugString(int indentation);
signals:
    
public slots:
    
private:
    QMap<QString, IXMLContentPtr > m_Content;
};

typedef QSharedPointer<XMLCollection> XMLCollectionPtr;
#endif // XMLCOLLECTION_H
