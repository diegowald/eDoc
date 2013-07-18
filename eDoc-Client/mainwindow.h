#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    IDocID *addDocument(const QByteArray &blob);
private slots:
    void on_pushButton_pressed();
    void on_LogTrace(const QString& text);
    void on_LogDebug(const QString& text);
    void on_LogInfo(const QString& text);
    void on_LogWarning(const QString& text);
    void on_LogError(const QString& text);
    void on_LogFatal(const QString& text);
    void on_pushButton_2_pressed();

private:
    Ui::MainWindow *ui;
    EDocFactory f;
    QObjectLogging logger;
};

#endif // MAINWINDOW_H
