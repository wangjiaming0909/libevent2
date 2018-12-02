#include "ranger_downloader.h"
#include <QException>

RangeDownloader::RangeDownloader(
        const ConfigContents* config,
        const QUrl &url,
        const Range& range,
        int index,
        QObject *parent)
    : QObject (parent),
      range_(range),
      url_(url),
      totalLength_(0),
      fileName_(),
      index_(index),
      config_(config)
{
    contents_ = std::shared_ptr<QByteArray>(new QByteArray{});
    QString scheme = url.scheme();
    manager_ = std::shared_ptr<QNetworkAccessManager>(new QNetworkAccessManager{});
    if(scheme.compare("https", Qt::CaseInsensitive) == 0)
        manager_->connectToHostEncrypted(url_.host());
    else if(scheme.compare("http", Qt::CaseInsensitive) == 0)
        manager_->connectToHost(url.host());
    request_ = std::shared_ptr<QNetworkRequest>(new QNetworkRequest{url.toString(QUrl::None)});
    request_->setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request_->setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::UserVerifiedRedirectPolicy);
    qDebug() << "url:" << url.toString();
    qDebug() << "url none:" << url.toString(QUrl::None);
}

void RangeDownloader::parseConfig()
{
    if(config_ == nullptr) {
        qDebug() << "no config pointer set";
        return;
    }
    if(!config_->url.startsWith(url_.scheme() + "://" + url_.host())){
        qDebug() << "config url is not the same as url_ ";
        qDebug() << "config url: " << config_->url;
        qDebug() << "url: " << url_.url();
        return;
    }
}

void RangeDownloader::setConfigHeaders()
{
    for(auto pair : config_->headers){
        setRequestHeader(pair.first, pair.second);
    }
    if(!config_->cookies.isEmpty()){
        QList<QNetworkCookie> cookies{};
        for(auto& cookie : config_->cookies){
            cookies.append(cookie);
            qDebug() << "setting cookie:" << cookie.name() << cookie.value();
        }
        request_->setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookies));
    }
}

void RangeDownloader::start(){
    if(config_ != nullptr){
        parseConfig();
        setConfigHeaders();
    }
    setRangeHeader();
    reply_ = manager_->get(*request_);
    connect(reply_, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply_, SIGNAL(redirected(QUrl)), this, SLOT(requestRedirected(QUrl)));
}

void RangeDownloader::setRequestHeader(const QString &headerName, const QString &value) {
    QNetworkRequest::KnownHeaders known_header;
    try {
        known_header = demultiplex_known_headers(headerName);
//        if(known_header == QNetworkRequest::CookieHeader){
//            QList<QVariant> cookies{};
//            for(auto& cookie : config_->cookies){
//                cookies.append(QVariant::fromValue(cookie));
//            }
//            request_->setHeader(known_header, cookies);
//            return;
//        }
        request_->setHeader(known_header, value);
    } catch (QException e) {
        request_->setRawHeader(headerName.toLatin1(), value.toLatin1());
    }
}

void RangeDownloader::setRangeHeader() {
    QByteArray start = QByteArray::number(range_.start);
    QByteArray end = QByteArray::number(range_.end);
    QByteArray value{};
    value = "bytes=" + start + "-" + end;
//    qDebug() << value;
    emit dump(QString("%1 %2").arg(index_).arg(QString(value)));
    request_->setRawHeader("Range", value);
}

void RangeDownloader::setTotalLength() {
    auto value = reply_->rawHeader("Content-Range");
    if(value.isEmpty()) return;
    int index = value.indexOf('/');
    totalLength_ = value.right(value.size() - index - 1).toInt();
//    qDebug() << "total length: " << totalLength_;
    emit dump(QString("%1 %2%3").arg(index_).arg("total length: ").arg(totalLength_));
}

void RangeDownloader::setFileName() {
    auto value = reply_->rawHeader("Content-Disposition");
    if(value.isEmpty()) {
        fileName_ = "tmp";
        return;
    }
    int indexOfEqual = value.indexOf('=');
    fileName_ = value.right(value.size() - indexOfEqual - 1);
    if(fileName_.startsWith("\"") && fileName_.endsWith("\"")){
        fileName_.remove(0, 1);
        fileName_.remove(fileName_.size()-1, 1);
    }
//    qDebug() << "fileName: " << fileName_;
    emit dump(QString("%1 %2%3").arg(index_).arg("fileName: ").arg(fileName_));
}


void RangeDownloader::downloadFinished() {
    contents_->clear();
    *contents_ = reply_->readAll();
//    qDebug() << "read: " << contents_->size() << "bytes";
    emit dump(QString("%1 %2%3").arg(index_).arg("read: ").arg(contents_->size()));
    if(first_tag_ == 0){
        setTotalLength();
        setFileName();
        first_tag_++;
    }
    emit finished(contents_, index_);
}

void RangeDownloader::requestRedirected(QUrl url)
{
    qDebug() << "reditected:" << url.toString();
    emit reply_->redirectAllowed();
}

QNetworkRequest::KnownHeaders demultiplex_known_headers(const QString &headerName) {
    if(headerName.compare("ContentType", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentTypeHeader;
    else if(headerName.compare("ContentLength", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentLengthHeader;
    else if(headerName.compare("Location", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::LocationHeader;
    else if(headerName.compare("LastModified", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::LastModifiedHeader;
    else if(headerName.compare("Cookie", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::CookieHeader;
    else if(headerName.compare("SetCookie", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::SetCookieHeader;
    else if(headerName.compare("ContentDisposition", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentDispositionHeader;
    else if(headerName.compare("User-Agent", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::UserAgentHeader;
    else if(headerName.compare("Server", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ServerHeader;
    else
        throw QException();
}
