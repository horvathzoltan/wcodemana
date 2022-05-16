#ifndef WCODE_H
#define WCODE_H

#include <QMap>
#include <QStringList>

class Wcode
{
public:
    QString wcode;
    QString tr_hu;
    QString tr_en;
    QString tr_de;

    static QMap<QString,Wcode> FromCSV(const QStringList& wcodes);
    void UpdateMessage(const QString& lang, const QString& msg);
    static QStringList ToCSV(const QMap<QString, Wcode> &wcodes);
    QStringList ToCSV() const;
    QStringList ToCode() const;
    static QStringList ToCode(const QMap<QString, Wcode> &wcodes);

    bool isValid(){ return !wcode.isEmpty();}
};

#endif // WCODE_H
