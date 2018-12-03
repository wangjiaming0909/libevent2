#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "downloader.h"
#include <vector>
#include <memory>
#include "downloadercontroller.h"
#include "ranger_downloader.h"
#include "filebuffer.h"
#include "configparser.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void get_1byte_and_total_length_and_fileName(const QUrl&, int from, int to);

private:
    QUrl check_url();
private slots:
    void on_browser_button__clicked();
    void on_download_button__clicked();
    void on_range_downloaded(std::shared_ptr<QByteArray>, int index, int ret);
    void on_1byte_range_downloaded(std::shared_ptr<QByteArray>, int index, int ret);
    void on_dump(QString info);
    void clear_state();
    void on_clear_button_clicked();
    void on_config_file_button_clicked();

private:
    Ui::Widget*                                     ui;
//    DownloaderController                            dl_controller_;


    QString                                         save_dir;
    std::vector<std::shared_ptr<RangeDownloader>>   range_downloaders_;
    std::shared_ptr<RangeDownloader>                first_range_downloader_;
    int                                             total_length_;
    QString                                         fileName_;
    std::shared_ptr<FileBuffer>                     fileBuffer_;
    std::vector<int>                                finished_flag_;
    QString                                         config_file_name_;
    ConfigContents                                  config_;

};

#endif // WIDGET_H
