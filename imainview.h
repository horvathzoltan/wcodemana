#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "mainviewmodel.h"

#include <QColor>
#include <QPointF>
#include <QRect>
#include <QSet>

class IMainView
{
public:
    virtual void set_DoWorkRModel(const MainViewModel::DoWorkRModel& m) = 0;
    virtual MainViewModel::DoWorkModel get_DoWorkModel() =0;

    virtual MainViewModel::ListItemChangedModel get_SelectedWcode() = 0;
    virtual MainViewModel::TextModel get_EnText() = 0;
    virtual MainViewModel::TextModel get_HuText() = 0;

    virtual void set_MessageEditor(const MainViewModel::ListItemChangedModelR &m) = 0;
    virtual MainViewModel::ListItemChangedModelR get_MessageEditor() = 0;
    virtual void set_RogzitStatus(MainViewModel::RogzitStatusR m) = 0;
    virtual void set_SaveStatus(bool isOk) = 0;
    virtual void set_SaveToCodeStatus(bool isOk) = 0;       
    virtual void set_GenerateResult(const MainViewModel::GenerateR& m) = 0;    
    virtual void set_EnToDeResult(const MainViewModel::GenerateR &m) = 0;
    virtual void set_HuToEnResult(const MainViewModel::GenerateR &m) = 0;

    virtual void GenerateTriggered(IMainView *sender) = 0;
    virtual void EnToDeTriggered(IMainView *sender) = 0;
    virtual void HuToEnTriggered(IMainView *sender) = 0;
public: // signals
    virtual void PushButtonActionTriggered(IMainView *sender) = 0;
    virtual void ListItemChangedTriggered(IMainView *sender) = 0;
    virtual void RogzitTriggered(IMainView *sender) = 0;
    virtual void SaveTriggered(IMainView *sender) = 0;
    virtual void SaveToCodeTriggered(IMainView *sender) =0;
};

#endif // IMAINVIEW_H
