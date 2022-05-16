#include "mainpresenter.h"
#include "mainviewmodel.h"
#include "dowork.h"

#include <QFileDialog>
#include <QDateTime>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);

    QObject::connect(view_obj, SIGNAL(PushButtonActionTriggered(IMainView *)),
                     this, SLOT(processPushButtonAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(ListItemChangedTriggered(IMainView *)),
                     this, SLOT(processListItemChangedAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(RogzitTriggered(IMainView *)),
                     this, SLOT(RogzitAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(SaveTriggered(IMainView *)),
                     this, SLOT(SaveAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(SaveToCodeTriggered(IMainView *)),
                     this, SLOT(SaveToCodeAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(GenerateTriggered(IMainView *)),
                     this, SLOT(GenerateAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(EnToDeTriggered(IMainView *)),
                     this, SLOT(EnToDeAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(HuToEnTriggered(IMainView *)),
                     this, SLOT(HuToEnAction(IMainView *)));
    //refreshView(w);
}

void MainPresenter::refreshView(IMainView *w) const { Q_UNUSED(w) };


auto MainPresenter::init(DoWork::Params params) -> bool
{
    _isInited = false;
    _w.init(params);
    connect(&_w,SIGNAL(ResponseOkAction2(QString)),
            this,SLOT(onResponseOkAction2(QString)));

    //_result = { Result::State::NotCalculated, -1};
    _isInited = true;
    return true;
}

void MainPresenter::AutoStartAction(IMainView *sender)
{
    MainPresenter::processPushButtonAction(sender);
}

void MainPresenter::initView(IMainView *w) const {
    MainViewModel::DoWorkRModel rm{"started",{}};
    w->set_DoWorkRModel(rm);
};

void MainPresenter::processPushButtonAction(IMainView *sender){
    qDebug() << "processPushButtonAction";
    auto m = sender->get_DoWorkModel();
    //DoWork w;
    auto rm = _w.Work1(m);
    sender->set_DoWorkRModel(rm);
}

void MainPresenter::processListItemChangedAction(IMainView *sender)
{
    qDebug() << "processListItemChangedAction";
    auto m = sender->get_SelectedWcode();
    auto rm = _w.GetSelected(m);
    sender->set_MessageEditor(rm);
}

void MainPresenter::RogzitAction(IMainView *sender)
{
    qDebug() << "RogzitAction";
    auto m = sender->get_MessageEditor();
    bool isOk = _w.SetSelected(m);
    sender->set_RogzitStatus(isOk);
}

void MainPresenter::SaveAction(IMainView *sender)
{
    qDebug() << "SaveAction";
    bool isOk = _w.SaveCSV();
    sender->set_SaveStatus(isOk);
}

void MainPresenter::SaveToCodeAction(IMainView *sender)
{
    qDebug() << "SaveToCodeAction";
    bool isOk = _w.SaveToCode();
    sender->set_SaveToCodeStatus(isOk);
}

void MainPresenter::GenerateAction(IMainView *sender)
{
    qDebug() << "GenerateAction";
    auto m = sender->get_SelectedWcode();
    auto m2 = _w.GetSelected(m);
    if(!m2.wcode.isValid()) return;
    auto code = m2.wcode.ToCode().join('\n');
    MainViewModel::GenerateR rm = {code};
    sender->set_GenerateResult(rm);
}

//EnToDeAction
void MainPresenter::EnToDeAction(IMainView *sender)
{
    qDebug() << "EnToDeAction";
    auto m = sender->get_SelectedWcode();
    auto m2 = _w.GetSelected(m);
    if(!m2.wcode.isValid()) return;
    _enToDeActionSender = sender;
    _dest_lang = "de";
    QString msg_de = _w.DeepLTranslate("en", "de", m2.wcode.tr_en);
}

void MainPresenter::onResponseOkAction2(QString msg_de)
{
    MainViewModel::GenerateR rm = {msg_de};
    if(_enToDeActionSender){
        if(_dest_lang == "de")
            _enToDeActionSender->set_EnToDeResult(rm);
        else if(_dest_lang == "en")
            _enToDeActionSender->set_HuToEnResult(rm);
    }
    _enToDeActionSender = nullptr;
    _dest_lang.clear();
}


void MainPresenter::HuToEnAction(IMainView *sender)
{
    qDebug() << "HuToEnAction";
    auto m = sender->get_SelectedWcode();
    auto m2 = _w.GetSelected(m);
    if(!m2.wcode.isValid()) return;
    _enToDeActionSender = sender;
    _dest_lang = "en";
    QString msg_de = _w.DeepLTranslate("hu", "en", m2.wcode.tr_hu);
}
