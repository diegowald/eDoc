#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include "../eDoc-API/IRecord.h"

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // RECORD_H
