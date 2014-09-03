#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/IDocID.h"
#include "../eDoc-ClientComponents/recordeditor.h"
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
    void fillSearchResultColumns();
    void fillSubTree(QTreeWidgetItem *parent);
    QList<QPair<QString, QString> > getTreeFilter(QTreeWidgetItem *parent);
    void doSearch(QList<QSharedPointer<IParameter>> &filter);
    QSharedPointer<IParameter> createSearchParameter(const QString &fieldName, VALIDQUERY queryType, QVariant value1, QVariant value2);
    void updateTreeFilter(QTreeWidgetItem *node);

    void openDatabase(const QString &file);

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

    void downloadFile(IRecordPtr record, const IValuePtr value);
    void uploadFile(QSharedPointer<IRecord> record, const QSharedPointer<IValue> value);
    void on_cboTree_currentIndexChanged(const QString &arg1);

    void on_treeStructure_itemSelectionChanged();

    void on_actionAdd_1000_Documents_triggered();

    void on_btnBrowse_pressed();

    void on_actionSet_Query_Date_and_Time_triggered();

    void on_actionImport_Folder_triggered();

    void on_actionOpen_database_triggered();

private:
    Ui::ExplorerWindow *ui;
    EDocFactory f;
    QSharedPointer<QObjectLogging> logger;
    QList<QSharedPointer<IParameter>> searchFilter;
    QList<QSharedPointer<IParameter>> treefilter;
    RecordEditor *recordEditor;
    bool useCurrentTime;
    QDateTime dateTimeToUse;
    QStringList columns;
};

#endif // EXPLORERWINDOW_H
