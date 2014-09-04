#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <QObject>

class FileManagement : public QObject
{
    Q_OBJECT
public:
    explicit FileManagement(const QString &folder, QObject *parent = 0);
    void createFile(QString fileName, const QByteArray &blob);
    QByteArray readFile(QString fileName);
    long size(QString filename);
signals:
    
public slots:
    
private:
    QString m_Folder;
};

#endif // FILEMANAGEMENT_H
