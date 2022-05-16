#include <QApplication>
#include <QClipboard>
#include <QtCharts>

#include "mainwindow.h"
#include "mainpresenter.h"

#include "common/logger/log.h"
#include "common/helper/signalhelper/signalhelper.h"
#include "dowork.h"


auto main(int argc, char *argv[]) -> int
{   
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGINT_); // shut down on ctrl-c
    com::helper::SignalHelper::setShutDownSignal(com::helper::SignalHelper::SIGTERM_); // shut down on killall

#if defined (STRING) && defined (TARGI)
    auto poj = STRING(TARGI);
    zInfo(QStringLiteral("started ")+poj);
#else
    auto poj=QStringLiteral("manymany");
#endif

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(poj);

    DoWork::Params params = DoWork::Params::Parse(app);

    MainWindow w;
    MainPresenter p;
    p.init(params);
    p.appendView(&w);
    w.show();    
    p.initView(&w);

    p.AutoStartAction(&w);

    return app.exec();
}
