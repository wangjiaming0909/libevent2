#include "downloader.h"
#include <QException>


Downloader::Downloader(const QString& url, int thread_count)
    : thread_count_(thread_count){
    manager_ = new QNetworkAccessManager();
    request_ = new QNetworkRequest(url);
}

Downloader::~Downloader() {
    if(manager_ != nullptr) delete manager_;
    if(request_ != nullptr) delete request_;
    if(reply_ != nullptr) delete reply_;
}

void Downloader::set_request_header(
        const QByteArray &key,
        const QByteArray &value) {
    request_->setRawHeader(key, value);
}

QByteArray Downloader::get_response_header(const QByteArray &headerName) const {
    if(reply_ == nullptr) return QByteArray();
    if(!reply_->isFinished()) return QByteArray();
    auto value = reply_->rawHeader(headerName);
    if(value.isEmpty())
        return get_known_header(demultiplex_known_headers(headerName));
    return value;
}

QByteArray Downloader::get_known_header(QNetworkRequest::KnownHeaders headerName) const{
    if(reply_ == nullptr) return QByteArray();
    if(!reply_->isFinished()) return QByteArray();
    QVariant value =  reply_->header(headerName);
    return value.toByteArray();
}

int Downloader::start() {
    manager_->connectToHostEncrypted(request_->url().host());
    reply_ = manager_->get(*request_);
    connect(reply_, SIGNAL(finished()), this, SLOT(on_read_finished()));
    contents_ = std::shared_ptr<QByteArray>(new QByteArray());
    return 0;
}

QString Downloader::dump() const {
    if(reply_ == nullptr) return QByteArray();
    if(!reply_->isFinished()) return QByteArray();
    QString str{};
    str.append("ContentType: " );
    str += get_known_header(QNetworkRequest::ContentTypeHeader);
    str += "\n";
    str.append("ContentLength: " );
    str += get_known_header(QNetworkRequest::ContentLengthHeader);
    str += "\n";
    str.append("Location: " );
    str += get_known_header(QNetworkRequest::LocationHeader);
    str += "\n";
    str.append("Cookie: " );
    str += get_known_header(QNetworkRequest::CookieHeader);
    str += "\n";
    str.append("SetCookie: " );
    str += get_known_header(QNetworkRequest::SetCookieHeader);
    str += "\n";
    str.append("UserAgent: " );
    str += get_known_header(QNetworkRequest::UserAgentHeader);
    str += "\n";
    str.append("Server: " );
    str += get_known_header(QNetworkRequest::ServerHeader);
    str += "\n";
    auto rawHeaders = reply_->rawHeaderList();
    for(auto header : rawHeaders){
        str += header;
        str += ": ";
        str += reply_->rawHeader(header) + "\n";
    }
    return str;
}

int Downloader::get_total_length() const {
    if(reply_ == nullptr) return 0;
    if(!reply_->isFinished()) return 0;
    auto data =  reply_->peek(1);
    qDebug() << data;
    auto value =  get_response_header("Content-Range");
    int index = value.indexOf('/');
    auto length = value.right(value.size() - index - 1).toInt();
    return length;
}

QNetworkRequest::KnownHeaders Downloader::demultiplex_known_headers(const QByteArray &headerName) const {
    QString header_str = headerName;
    if(header_str.compare("ContentType", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentTypeHeader;
    else if(header_str.compare("ContentLength", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentLengthHeader;
    else if(header_str.compare("Location", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::LocationHeader;
    else if(header_str.compare("LastModified", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::LastModifiedHeader;
    else if(header_str.compare("Cookie", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::CookieHeader;
    else if(header_str.compare("SetCookie", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::SetCookieHeader;
    else if(header_str.compare("ContentDisposition", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ContentDispositionHeader;
    else if(header_str.compare("UserAgent", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::UserAgentHeader;
    else if(header_str.compare("Server", Qt::CaseInsensitive) == 0)
        return QNetworkRequest::ServerHeader;
    else
        throw QException();
}


void Downloader::on_read_finished() {
    *contents_ = reply_->readAll();
//    qDebug() << *contents_;
    qDebug() << dump();
    emit download_finished(this);
}



