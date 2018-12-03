#ifndef RANGEDOWNLOADER_H_
#define RANGEDOWNLOADER_H_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <memory>
#include "configparser.h"

class RangeDownloader : public QObject {
    Q_OBJECT
public:
    struct Range{
        Range(int s, int e) : start(s), end(e){}
        int start;
        int end;
    };
    explicit RangeDownloader(const ConfigContents* config, const QUrl& url, const Range& range, int index, QObject* parent = nullptr);
    std::shared_ptr<QByteArray> get_contents() const{return contents_;}
    int TotalLength() const {return totalLength_; }
    QString getFileName() {return  fileName_; }
    void start();
    void setRequestHeader(const QString& headerName, const QString& value);

private:
    void setRangeHeader();
    void setTotalLength();
    void setFileName();
    void parseConfig();
    void setConfigHeaders();
signals:
    void finished(std::shared_ptr<QByteArray>, int, int ret_value);
    void dump(QString info);
public slots:
    void downloadFinished();
    void requestRedirected(QUrl);
private:
    std::shared_ptr<QNetworkAccessManager>              manager_;
    std::shared_ptr<QNetworkRequest>                    request_;
    QNetworkReply*                                      reply_;

private:
    std::shared_ptr<QByteArray>         contents_;
    Range                               range_;
    QUrl                                url_;
    int                                 totalLength_;
    QString                             fileName_;
    int                                 index_;
    int                                 first_tag_ = 0;
    const ConfigContents*               config_;
};

QNetworkRequest::KnownHeaders demultiplex_known_headers(const QString &headerName);

#endif // RANGEDOWNLOADER_H_
