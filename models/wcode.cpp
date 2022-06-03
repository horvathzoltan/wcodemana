#include "wcode.h"
#include "common/logger/log.h"

/*
WC_UI_Web_Role.Label.Description	de-DE	Description
WC_UI_Web_Role.Label.Description	en-US	Description
WC_UI_Web_Hardware.Label.Descriptor	en-US	Descriptor
WC_UI_Web_Hardware.Label.Descriptor	hu-HU	Descriptor
*/
QMap<QString, Wcode> Wcode::FromCSV(const QStringList &wcodes)
{
    QMap<QString, Wcode> m;
    for(auto &a:wcodes){
        if(!a.startsWith("WC")) continue;
        int ix1 = a.indexOf('\t');
        if(ix1==-1) continue;
        int ix2 = a.indexOf('\t', ix1+1);
        if(ix2==-1) continue;
        QString wc = a.left(ix1);
        QString lang= a.mid(ix1+1,ix2-ix1-1);
        QString msg= a.mid(ix2+1);

        zInfo("wc: "+wc);

        if(m.contains(wc)){
            m[wc].UpdateMessage(lang,msg);
        } else{
            Wcode wcode;
            wcode.wcode=wc;
            wcode.UpdateMessage(lang,msg);
            m.insert(wc, wcode);
        }
    }
    return m;
}

QStringList Wcode::ToCSV(const QMap<QString, Wcode> &wcodes)
{
    QStringList m;
    for(auto &a:wcodes) m.append(a.ToCSV());
    return m;
}

QStringList Wcode::ToCode(const QMap<QString, Wcode> &wcodes)
{
    QStringList m;
    for(auto &a:wcodes) m.append(a.ToCode());
    return m;
}

QStringList Wcode::ToCSV() const
{
    QStringList m;
    m.append(wcode+"\thu-HU\t"+tr_hu);
    m.append(wcode+"\ten-US\t"+tr_en);
    m.append(wcode+"\tde-DE\t"+tr_de);
    return m;
}

void Wcode::UpdateMessage(const QString& lang, const QString& msg){
    if(lang=="hu-HU") tr_hu = msg;
    if(lang=="en-US") tr_en = msg;
    if(lang=="de-DE") tr_de = msg;
}

/*
    [Translation("", Lang.HU)]
    [Translation("Unable to save unloaded insole pressure values to file", Lang.EN)]
    [Translation("", Lang.DE)]
    public static class UnableToSave { }
*/

QStringList Wcode::ToCode() const
{
    QStringList m;
    m.append("[Translation(\""+tr_hu+"\", Lang.HU)]");
    m.append("[Translation(\""+tr_en+"\", Lang.EN)]");
    m.append("[Translation(\""+tr_de+"\", Lang.DE)]");
    m.append("public static class "+wcode+" { }");
    m.append("");
    return m;
}

void Wcode::AssertByUsing(QMap<QString, Wcode>* wcodes, const QStringList& used)
{
    QString lastFilePath;
    for(auto& u:used){
        if(u.isEmpty()) continue;
        if(u.startsWith("##")){
            int ix1 = u.indexOf('/');
            if(ix1>-1){
                int ix2 = u.indexOf('#', ix1);
                if(ix2>-1 && ix2>ix1){
                    lastFilePath = u.mid(ix1, ix2-ix1);
                }
            }
        } else if(!u[0].isDigit()) continue;
        int ix1 = u.indexOf(':');
        if(ix1>-1){
            QString ln = u.left(ix1);
            int ix2 = u.indexOf('\"', ix1+1);
            if(ix2>-1){
                int ix3 = u.indexOf('\"', ix2+1);
                if(ix3>-1&&ix3>ix2){
                    bool ok;
                    int lineNumber = ln.toInt(&ok);
                    QString wc = u.mid(ix2+1, ix3-ix2-1);
                    if(ok){
                        if(wcodes->contains(wc)){
                            Wcode *w = &(*wcodes)[wc];
                            w->isUsed=true;
                            w->fileName = lastFilePath;
                            w->lineNumber = lineNumber;
                        }else{
                            Wcode w;
                            w.wcode=wc;
                            w.isUsed = true;
                            w.fileName = lastFilePath;
                            w.lineNumber = lineNumber;
                            wcodes->insert(wc, w);
                        }
                    }
                }
            }
        }
    }
}
