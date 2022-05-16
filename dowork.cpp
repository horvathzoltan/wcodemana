#include "dowork.h"
#include "httphelper.h"

#include "helpers/filenamehelper.h"
#include "common/logger/log.h"
#include "common/helper/textfilehelper/textfilehelper.h"
#include "common/helper/CommandLineParserHelper/commandlineparserhelper.h"
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "models/wcode.h"

DoWork::DoWork(QObject *parent) :QObject(parent)
{

}

auto DoWork::Params::Parse(const QCoreApplication& app) -> DoWork::Params
{
    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_IN = QStringLiteral("input");
    const QString OPTION_OUT = QStringLiteral("output");
    const QString OPTION_BACKUP = QStringLiteral("backup");
    const QString OPTION_TEST = QStringLiteral("test");

    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_IN, QStringLiteral("geometry file as input"));
    com::helper::CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("g-code file as output"));
    com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));
    com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_TEST, QStringLiteral("set to activate test mode"));

    parser.process(app);

    return {
        parser.value(OPTION_IN),
        parser.value(OPTION_OUT),
        parser.isSet(OPTION_BACKUP),
        parser.isSet(OPTION_TEST)
    };
}

auto DoWork::Params::IsValid() -> bool
{
    QStringList err;
//    if(inFile.isEmpty())
//    {
//        err.append(QStringLiteral("inFile is empty"));
//    }
//    if(inFile.isEmpty())
//    {
//        err.append(QStringLiteral("outFile is empty"));
//    }
    if(!err.isEmpty()) zInfo(err)
            return err.isEmpty();
}


auto DoWork::init(DoWork::Params p) -> bool
{
    _isInited = false;
    if(!p.IsValid()) return false;

    params = p;    

    //_result = { Result::State::NotCalculated, -1};
    QObject::connect(&_h, SIGNAL(ResponseOk(QByteArray)),
                     this, SLOT(ResponseOkAction(QByteArray)));

    _isInited = true;
    return true;
}


auto DoWork::GetWorkingFolder() -> QString
{
    QString workingFolder = params.isTest?FileNameHelper::GetTestFolderPath():qApp->applicationDirPath();
    return workingFolder;
}

auto DoWork::Work1(MainViewModel::DoWorkModel m) -> MainViewModel::DoWorkRModel
{
    if(!_isInited) return {QStringLiteral("not inited"),{}};

    _wcodes.clear();

    QString workingFolder = GetWorkingFolder();

    if(params.inFile.isEmpty()){
        return {QStringLiteral("no input file"),{}};
    }

    if(params.outFile.isEmpty()){
        QFileInfo fi(params.inFile);
        QString bn = fi.baseName();
        params.outFile=bn + ".txt";
    }
    zInfo("workingFolder: "+workingFolder);
    zInfo(QStringLiteral("params: %1, %2, %3, %4")
              .arg(params.inFile)
              .arg(params.outFile)
              .arg(params.isBackup)
              .arg(params.isTest));

    auto d = QDir(workingFolder);

    auto file = d.filePath(params.inFile);
    auto wcodes_csv = com::helper::TextFileHelper::loadLines(file);

    if(wcodes_csv.isEmpty()) return {QStringLiteral("file is empty"),{}};

    _wcodes = Wcode::FromCSV(wcodes_csv);

    //return {QString::number(m.i+1)};
    return
    {
        QStringLiteral("lines: ")+QString::number(_wcodes.count()),
                _wcodes
    };
}

auto DoWork::GetSelected(MainViewModel::ListItemChangedModel m) -> MainViewModel::ListItemChangedModelR
{
    if(!_wcodes.contains(m.selectedItemKey)) return {};
    auto a = _wcodes[m.selectedItemKey];
    return {a};
}

bool DoWork::SetSelected(const MainViewModel::ListItemChangedModelR &m)
{
    if(!_wcodes.contains(m.wcode.wcode)) return false;
    _wcodes[m.wcode.wcode]=m.wcode;
    return true;
}

bool DoWork::SaveCSV(){

    if(!_isInited) return false;

    QString workingFolder = GetWorkingFolder();

    zInfo("workingFolder: "+workingFolder);
    zInfo(QStringLiteral("params: %1, %2, %3, %4")
              .arg(params.inFile)
              .arg(params.outFile)
              .arg(params.isBackup)
              .arg(params.isTest));

    auto d = QDir(workingFolder);
    auto file = d.filePath(params.outFile);
    QStringList wcodes_csv = Wcode::ToCSV(_wcodes);
    bool isOk = com::helper::TextFileHelper::save(wcodes_csv.join('\n'),file);

    return isOk;
}

bool DoWork::SaveToCode(){

    if(!_isInited) return false;

    QString workingFolder = GetWorkingFolder();

    zInfo("workingFolder: "+workingFolder);
    zInfo(QStringLiteral("params: %1, %2, %3, %4")
              .arg(params.inFile)
              .arg(params.outFile)
              .arg(params.isBackup)
              .arg(params.isTest));

    auto fn = params.outFile.replace(".txt",".cs");
    auto d = QDir(workingFolder);
    auto file = d.filePath(fn);
    QStringList wcodes_csv = Wcode::ToCode(_wcodes);
    bool isOk = com::helper::TextFileHelper::save(wcodes_csv.join('\n'),file);

    return isOk;
}

QString DoWork::DeepLTranslate(const QString &source_lang,
                               const QString &dest_lang,
                               const QString &msg)
{
    _h.SendPost(source_lang, dest_lang,msg);
    zInfo("request sent");
    return QString("request sent");
}

void DoWork::ResponseOkAction(QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    auto tr = rootobj.value("translations").toArray();
    QString translation;
    foreach(const QJsonValue & val, tr){
        translation=val.toObject().value("text").toString();
    }

    zInfo("ResponseOkAction:" + translation);

    emit ResponseOkAction2(translation);
}
