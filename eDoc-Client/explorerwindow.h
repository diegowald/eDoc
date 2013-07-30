#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include <QMainWindow>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"

namespace Ui {
class ExplorerWindow;
}

class ExplorerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ExplorerWindow(QWidget *parent = 0);
    ~ExplorerWindow();

private:
    void fillFieldsCombo();
    void fillOperatorsCombo();
    
private slots:
    void on_LogTrace(const QString& text);
    void on_LogDebug(const QString& text);
    void on_LogInfo(const QString& text);
    void on_LogWarning(const QString& text);
    void on_LogError(const QString& text);
    void on_LogFatal(const QString& text);

    void on_cboOperator_currentIndexChanged(int index);

    void on_btnAddToSearch_released();

    void on_btnSearchAgain_released();

    void on_btnClearSearch_released();

    void on_searchResult_itemSelectionChanged();

private:
    Ui::ExplorerWindow *ui;
    EDocFactory f;
    QObjectLogging logger;
    QList<IParameter *> filter;
};

#endif // EXPLORERWINDOW_H
