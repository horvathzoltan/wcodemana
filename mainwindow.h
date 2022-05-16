#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QClipboard>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QTimer>
#include "imainview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QTimer _statusLabelTimer;

    void set_DoWorkRModel(const MainViewModel::DoWorkRModel& m);
    MainViewModel::DoWorkModel get_DoWorkModel();
    void ShowStatusMsg(QString msg);
    void ClearStatusMsg();
    void AppendCodeEditor(QString msg);
    QClipboard *_clipboard;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainViewModel::ListItemChangedModel get_SelectedWcode();
    void set_MessageEditor(const MainViewModel::ListItemChangedModelR &m);
    MainViewModel::ListItemChangedModelR get_MessageEditor();
    void set_RogzitStatus(bool isOk);
    void set_SaveStatus(bool isOk);
    void set_SaveToCodeStatus(bool isOk);
    void closeEvent(QCloseEvent *event);
    void set_GenerateResult(const MainViewModel::GenerateR& m);
    void set_EnToDeResult(const MainViewModel::GenerateR &m);
    void set_HuToEnResult(const MainViewModel::GenerateR &m);

signals:
    void PushButtonActionTriggered(IMainView *sender);
    void ListItemChangedTriggered(IMainView *sender);
    void RogzitTriggered(IMainView *sender);
    void SaveTriggered(IMainView *sender);
    void SaveToCodeTriggered(IMainView *sender);
    void GenerateTriggered(IMainView *sender);
    void EnToDeTriggered(IMainView *sender);
    void HuToEnTriggered(IMainView *sender);

private slots:    
    void on_pushButton_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_ok_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_code_clicked();
    void on_lineEdit_hu_textChanged(const QString &arg1);
    void on_pushButton_clear_clicked();
    void on_pushButton_generate_clicked();
    void on_pushButton_copytoclipboard_clicked();
    void on_pushButton_en_to_de_clicked();
    void on_pushButton_hu_to_en_clicked();
};
#endif // MAINWINDOW_H
