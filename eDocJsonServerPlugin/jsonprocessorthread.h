#ifndef JSONPROCESSORTHREAD_H
#define JSONPROCESSORTHREAD_H

#include <QThread>

class JSonProcessorThread : public QThread
{
    Q_OBJECT
public:
    explicit JSonProcessorThread(QObject *parent = 0);

signals:

public slots:

};

#endif // JSONPROCESSORTHREAD_H
