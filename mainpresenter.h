#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

//#include "actions.h"
//#include "updates.h"

#include <QList>
#include <QObject>
#include "imainview.h"
#include "dowork.h"

class IMainView;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject *parent = nullptr);
    void appendView(IMainView *w);
    void initView(IMainView *w) const;
    bool init(DoWork::Params params);
    void AutoStartAction(IMainView *sender);
private:
    QList<IMainView*> _views;
    void refreshView(IMainView *w) const;

private slots:
    void processPushButtonAction(IMainView *sender);
    void processListItemChangedAction(IMainView *sender);
    void RogzitAction(IMainView *sender);
    void SaveAction(IMainView *sender);
    void SaveToCodeAction(IMainView *sender);
    void GenerateAction(IMainView *sender);
    void EnToDeAction(IMainView *sender);
    void onResponseOkAction2(QString);
    void HuToEnAction(IMainView *sender);

private:
    DoWork _w;
    bool _isInited = false;
    IMainView *_enToDeActionSender = nullptr;
    QString _dest_lang;
};

#endif // MAINPRESENTER_H
