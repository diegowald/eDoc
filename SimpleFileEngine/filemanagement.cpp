#include "filemanagement.h"
#include <QFile>

FileManagement::FileManagement(const QString &folder, QObject *parent) :
    QObject(parent)
{
    m_Folder = folder;
}

void FileManagement::createFile(QString fileName, const QByteArray &blob)
{
    QString fn(m_Folder + "/" + fileName);
    QFile file(fn);
    file.open(QIODevice::WriteOnly);
    file.write(blob);
    file.close();
}

QByteArray FileManagement::readFile(QString fileName)
{
    QString fn(m_Folder + "/" + fileName);
    QFile file(fn);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}
