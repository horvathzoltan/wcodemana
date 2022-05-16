#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <QObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QUrl>

class HttpHelper : public QObject
{
    Q_OBJECT
public:
    explicit HttpHelper(QObject *parent = nullptr);

    void SendPost(const QString& source_lang, const QString& dest_lang, const QString& msg);

public slots:
    void onFinish(QNetworkReply *rep);
signals:
    void ResponseOk(QByteArray s);
};

#endif // HTTPHELPER_H
