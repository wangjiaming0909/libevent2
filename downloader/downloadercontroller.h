#ifndef DOWNLOADERCONTROLLER_H
#define DOWNLOADERCONTROLLER_H

#include <QUrl>

class DownloaderController {
public:
    DownloaderController(const QUrl& url, int connection_count);
    ~DownloaderController();
public:
    void start_download();
//    void set_save_dir();
//    void clear_state();
public:
//    void set_Url(const QUrl& url){url_ = url;}
//    void set_conection_count(int cc){connection_count_ = cc;}
//    void set_save_path(const QString& path){save_path_ = path;}
//    QString get_full_file_name() const {return file_full_name_;}

private:
//    QUrl            url_;
//    int             connection_count_;
//    QString         save_path_;
//    QString         file_full_name_;
//    QString         file_name_;

};

#endif // DOWNLOADERCONTROLLER_H
