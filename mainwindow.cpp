#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "imainview.h"

#include <QTimer>
#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);   
    _statusLabelTimer.setParent(this);
    _statusLabelTimer.setSingleShot(true);

    QObject::connect(&_statusLabelTimer, &QTimer::timeout, this, &MainWindow::ClearStatusMsg);

    _clipboard = QGuiApplication::clipboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_DoWorkRModel(const MainViewModel::DoWorkRModel& m)
{
    ui->label->setText(m.txt);
    ui->lineEdit_hu->clear();
    ui->lineEdit_en->clear();
    ui->lineEdit_de->clear();
    ui->listWidget->clear();

    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_DialogOkButton);

    auto keys = m.wcodes.keys();
    //ui->listWidget->addItems(keys);
    for(auto&k:keys){
        auto i = new QListWidgetItem(ui->listWidget, 0);
        i->setText(k);
        auto a = m.wcodes[k];
        if(a.isUsed){
            if(a.isInDb) i->setBackground(Qt::green);
            else i->setBackground(Qt::yellow);
        }

        if(a.isWcodeOk()) i->setIcon(icon);
        //i->setIcon()
        //ui->listWidget->addItem(&i);
    }

};



auto MainWindow::get_DoWorkModel() -> MainViewModel::DoWorkModel
{
    auto t = ui->label->text();
    bool isok;
    int i = t.toInt(&isok, 10);
    if(!isok) return {-1};
    return {i};
};

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "PushButtonActionTriggered";
    emit PushButtonActionTriggered(this);
}


auto MainWindow::get_SelectedWcode() -> MainViewModel::ListItemChangedModel
{
    auto t = ui->listWidget->currentItem();
    if(!t) return{};
    return {t->text()};
};


auto MainWindow::get_EnText() -> MainViewModel::TextModel
{
    auto t = ui->lineEdit_en->text();
    if(t.isEmpty()) return{};
    return {t};
};

auto MainWindow::get_HuText() -> MainViewModel::TextModel
{
    auto t = ui->lineEdit_hu->text();
    if(t.isEmpty()) return{};
    return {t};
};

void MainWindow::set_MessageEditor(const MainViewModel::ListItemChangedModelR& m)
{
    ui->label_wcode->setText(m.wcode.wcode);
    ui->lineEdit_de->setText(m.wcode.tr_de);
    ui->lineEdit_en->setText(m.wcode.tr_en);
    ui->lineEdit_hu->setText(m.wcode.tr_hu);
    QString msg = m.wcode.isUsed?(m.wcode.fileName+" ("+QString::number(m.wcode.lineNumber)+")"):"not used";
    ui->label_using->setText(msg);
}

void MainWindow::set_RogzitStatus(MainViewModel::RogzitStatusR m)
{
    ShowStatusMsg(QStringLiteral("Rögzítés ")+(m.isOk?"ok":"error"));
    AppendCodeEditor("code");
    auto items = ui->listWidget->findItems(m.wcode.wcode, Qt::MatchFlag::MatchExactly);
    if(items.isEmpty()) return;
    auto item = items.first();

    auto isOk = m.wcode.isWcodeOk();

    if(isOk) item->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOkButton)); else item->setIcon({});
}

void MainWindow::set_SaveStatus(bool isOk)
{
    ShowStatusMsg(QStringLiteral("Mentés ")+(isOk?"ok":"error"));
}

void MainWindow::set_SaveToCodeStatus(bool isOk)
{
    ShowStatusMsg(QStringLiteral("Kódgenerálás ")+(isOk?"ok":"error"));
}

MainViewModel::ListItemChangedModelR MainWindow::get_MessageEditor()
{
    MainViewModel::ListItemChangedModelR m;

    m.wcode.wcode = ui->label_wcode->text();
    m.wcode.tr_de = ui->lineEdit_de->text();
    m.wcode.tr_en = ui->lineEdit_en->text();
    m.wcode.tr_hu = ui->lineEdit_hu->text();

    return m;
};

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //qDebug() << "ListItemClicked";
    //emit RogzitTriggered(this);
    //emit ListItemChangedTriggered(this);
}


void MainWindow::on_pushButton_ok_clicked()
{
    qDebug() << "RogzitClicked";
    emit RogzitTriggered(this);
}


void MainWindow::on_pushButton_save_clicked()
{
    qDebug() << "SaveClicked";
    emit SaveTriggered(this);
}

void MainWindow::closeEvent( QCloseEvent *event )
{
    qDebug() << "closeEvent";
    //emit SaveTriggered(this);
}



void MainWindow::on_pushButton_code_clicked()
{
    qDebug() << "SaveToCodeClicked";
    emit SaveToCodeTriggered(this);
}


void MainWindow::on_lineEdit_hu_textChanged(const QString &arg1)
{
    //ui->label_status->setText(QStringLiteral("hu changed"));
}

void MainWindow::ShowStatusMsg(QString msg){
    if(msg.isEmpty()) ClearStatusMsg();
    else{
        ui->label_status->setText(msg);
        _statusLabelTimer.start(1500);
    }
}

void MainWindow::ClearStatusMsg(){
    ui->label_status->clear();
}


void MainWindow::on_pushButton_clear_clicked()
{
    ui->plainTextEdit->clear();
}


/*Generate*/

void MainWindow::on_pushButton_generate_clicked()
{
    qDebug() << "GenerateClicked";
    emit GenerateTriggered(this);
}

void MainWindow::set_GenerateResult(const MainViewModel::GenerateR& m)
{
    if(!m.code.isEmpty()) AppendCodeEditor(m.code);
}

void MainWindow::set_EnToDeResult(const MainViewModel::GenerateR& m)
{
    if(!m.code.isEmpty()) ui->lineEdit_de->setText(m.code);
}

void MainWindow::set_HuToEnResult(const MainViewModel::GenerateR& m)
{
    if(!m.code.isEmpty()) ui->lineEdit_en->setText(m.code);
}


void MainWindow::AppendCodeEditor(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void MainWindow::on_pushButton_copytoclipboard_clicked()
{
    auto txt = ui->plainTextEdit->toPlainText();
    if(txt.isEmpty()) return;
    _clipboard->setText(txt);
}


void MainWindow::on_pushButton_en_to_de_clicked()
{
    qDebug() << "EnToDeClicked";
    emit EnToDeTriggered(this);
}


void MainWindow::on_pushButton_hu_to_en_clicked()
{
    qDebug() << "HuToEnClicked";
    emit HuToEnTriggered(this);
}


void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    qDebug() << "ListItemCurrentItemChanged";
    //emit RogzitTriggered(this);
    emit ListItemChangedTriggered(this);
}

