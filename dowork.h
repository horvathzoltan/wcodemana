#ifndef DOWORK_H
#define DOWORK_H

#include "httphelper.h"
#include "mainviewmodel.h"

#include <QCoreApplication>

class DoWork: public QObject
{
    Q_OBJECT

public:
    explicit DoWork(QObject *parent = nullptr);

    static const QString COMORSTR;

    struct Params{
    public:
        QString inFile;
        QString outFile;
        QString uwcFile;
        bool isBackup;
        bool isTest = false;

        bool IsValid();

        static Params Parse(const QCoreApplication& app);
    };

    struct Result{
        enum State:int {NotCalculated = -3, NoResult=-1, Ok=1};
        State state;
        int value;

        QString ToString();
    };        

    bool init(Params p);
    MainViewModel::DoWorkRModel Work1(MainViewModel::DoWorkModel m);
    QString GetWorkingFolder();

    MainViewModel::ListItemChangedModelR GetSelected(MainViewModel::ListItemChangedModel m);
    bool SetSelected(const MainViewModel::ListItemChangedModelR& m);
    bool SaveCSV();
    bool SaveToCode();

    QString DeepLTranslate(const QString& source_lang, const QString& dest_lang, const QString& msg);

    QString ReplaceTr(const QString &msg);
private:
    bool _isInited = false;
    bool _isEventLoopNeeded = false;
    Params params;
    QMap<QString, Wcode> _wcodes;
    HttpHelper _h;
    //Result doWork2();

signals:
    void ResponseOkAction2(QString);

private slots:
    void ResponseOkAction(QByteArray s);

};

#endif // DOWORK_H
