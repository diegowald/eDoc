#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include <QMainWindow>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"
#include <QTreeWidgetItem>
#include <QList>
#include <QPair>

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
    void fillTreeCombo();
    void fillSubTree(QTreeWidgetItem *parent);
    QList<QPair<QString, QString> > getTreeFilter(QTreeWidgetItem *parent);
    void doSearch(QList<IParameter *> &filter);
    IParameter *createSearchParameter(const QString &fieldName, VALIDQUERY queryType, QVariant value1, QVariant value2);
    void updateTreeFilter(QTreeWidgetItem *node);

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

    void on_actionAdd_Document_triggered();

    void downloadFile(IRecord* record, const IValue *value);
    void uploadFile(IRecord* record, const IValue* value);
    void on_cboTree_currentIndexChanged(const QString &arg1);

    void on_treeStructure_itemSelectionChanged();

private:
    Ui::ExplorerWindow *ui;
    EDocFactory f;
    QObjectLogging logger;
    QList<IParameter *> searchFilter;
    QList<IParameter *> treefilter;
};

#endif // EXPLORERWINDOW_H
