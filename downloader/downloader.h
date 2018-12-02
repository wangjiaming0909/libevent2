#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <memory>

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(const QString& url, int thread_count);
    ~Downloader();
    void set_request_header(const QByteArray& headerName, const QByteArray& value);
    QByteArray get_response_header(const QByteArray& headerName) const;
    int start();
    const std::shared_ptr<QByteArray> get_contents() const{return contents_;}
    QString dump() const;
    int get_total_length() const;
private:
    QByteArray get_known_header(QNetworkRequest::KnownHeaders headerName) const ;
    QNetworkRequest::KnownHeaders demultiplex_known_headers(const QByteArray& headerName) const ;
    QString get_dump_string(const QString& headerName) const ;
private:
    int                             thread_count_;
    QNetworkAccessManager*          manager_;
    QNetworkRequest*                request_;
    QNetworkReply*                  reply_;
    std::shared_ptr<QByteArray>     contents_;
public slots:
    void on_read_finished();
Q_SIGNALS:
    void download_finished(Downloader*);
};

#endif // DOWNLOADER_H
