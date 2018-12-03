#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <algorithm>
#include <limits>
#include <thread>

const static QString url = "http://d1.music.126.net/dmusic/netease-cloud-music_1.1.0_amd64_ubuntu.deb";

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    total_length_(0),
    fileName_(),
    finished_flag_(),
    config_file_name_(),
    config_(),
    package_range_downloader_(nullptr),
    package_range_downloader_ptrs_()
{
    ui->setupUi(this);
    ui->lineEdit->setText(url);
    ui->spinBox->setRange(1, 1024);
    ui->spinBox->setValue(10);
    ui->start_spin_box->setRange(0, INT_LEAST32_MAX);
    ui->end_spin_box->setRange(0, INT_LEAST32_MAX);
    ui->index_spin_box->setReadOnly(true);
    ui->index_spin_box->setRange(0, INT_LEAST32_MAX);
    range_downloaders_ = std::vector<std::shared_ptr<RangeDownloader>>{10};
}

Widget::~Widget() {
    delete ui;
}

void Widget::on_browser_button__clicked() {
    this->save_dir = QFileDialog::getExistingDirectory();
    ui->dir_lineedit->setText(save_dir);
    qDebug() << save_dir;
}

void Widget::get_1byte_and_total_length_and_fileName(const QUrl& url, int from, int to) {
    first_range_downloader_ = std::shared_ptr<RangeDownloader>(new RangeDownloader{&config_, url, {from, to}, 0});
    connect(first_range_downloader_.get(),
            SIGNAL(finished(std::shared_ptr<QByteArray>, int, int)),
            this,
            SLOT(on_1byte_range_downloaded(std::shared_ptr<QByteArray>, int, int)));
    connect(first_range_downloader_.get(),
            SIGNAL(dump(QString)),
            this,
            SLOT(on_dump(QString)));
    first_range_downloader_->start();
    ui->content_editor->append("request sending...");
}

QUrl Widget::check_url() {
    QString url_qstr = ui->lineEdit->text();
    auto url = QUrl::fromUserInput(url_qstr);
    if(url.isEmpty())
        QMessageBox::warning(this, "url erro", "Invalid url " + url_qstr);
    return url;
}


void Widget::on_download_button__clicked() {
    request();
}

void Widget::on_range_downloaded(std::shared_ptr<QByteArray> data, int index, int ret) {
    fileBuffer_->write(data, index);
    if(index >= 0)
        finished_flag_[index] = 1;
    //means that the last connection has alreay get the data, and write it to the fileBuffer
    if(std::find(finished_flag_.begin(), finished_flag_.end(), 0) == finished_flag_.end()){
        ui->content_editor->append("file: " + fileName_ + " download completed...");
        clear_state();
    }
}

void Widget::clear_state() {
    range_downloaders_.clear();
    first_range_downloader_.reset();
    total_length_ = 0;
    fileName_.clear();
    fileBuffer_.reset();
    finished_flag_.clear();
    config_file_name_.clear();
    config_.clear();
    ui->lineEdit->clear();
    ui->dir_lineedit->clear();
    one_byte_data_->clear();
    package_range_downloader_ = nullptr;
    package_range_downloader_ptrs_.clear();
    ui->index_spin_box->setValue(0);
}

void Widget::on_1byte_range_downloaded(std::shared_ptr<QByteArray> data, int index, int ret) {
    total_length_ = first_range_downloader_->TotalLength();
    if(ret != 0 || total_length_ == 0){
        ui->content_editor->append("error, total_length is 0");
        clear_state();
        ui->content_editor->append("state cleared...");
        return;
    }
    fileName_ = first_range_downloader_->getFileName();
    QString fileFullName = save_dir + "/" + fileName_;
    ui->dir_lineedit->setText(fileFullName);
    fileBuffer_ = std::shared_ptr<FileBuffer>(new FileBuffer{fileFullName, 1000});
    one_byte_data_ = data;
}

void Widget::request() {
    fileBuffer_->write(one_byte_data_, -1);
    int package_size = total_length_ / range_downloaders_.size() + 1;
    int front = 0, end = 0;
    for(int i = 0; i < range_downloaders_.size(); i++){
        front = i * package_size + 1;
        end = (package_size * (i + 1)) > total_length_  ? total_length_ : package_size * (i + 1);
        range_downloaders_[i] = std::shared_ptr<RangeDownloader>(new RangeDownloader{&config_, check_url(), {front, end}, i});
        connect(range_downloaders_[i].get(), SIGNAL(finished(std::shared_ptr<QByteArray>, int, int)), this, SLOT(on_range_downloaded(std::shared_ptr<QByteArray>, int, int)));
        connect(range_downloaders_[i].get(), SIGNAL(dump(QString)), this, SLOT(on_dump(QString)));
        range_downloaders_[i]->start();
    }
}

void Widget::on_dump(QString info) {
    ui->content_editor->append(info);
}

void Widget::on_clear_button_clicked() {
    ui->content_editor->clear();
    clear_state();
}

void Widget::on_config_file_button_clicked() {
    config_file_name_ = QFileDialog::getOpenFileName(this, "Choose idm config file", "~/", "idm (*.ef2)");
    if(config_file_name_.isEmpty())
        return;
    IDMConfigParser parser{config_file_name_};
    config_ = *parser.getconfigs()[0];
    ui->lineEdit->setText(config_.url);

    auto url = check_url();
    if(url.isEmpty()) return;
    range_downloaders_.resize(ui->spinBox->text().toUInt());
    finished_flag_.resize(ui->spinBox->text().toUInt());
    for(int& i : finished_flag_) i = 0;
    get_1byte_and_total_length_and_fileName(url, 0, 0);
}

void Widget::on_request_button_clicked() {
    int index = ui->index_spin_box->value();
    RangeDownloader::Range r{ui->start_spin_box->value(), ui->end_spin_box->value()};
    package_range_downloader_ = new RangeDownloader(&config_, check_url(), r, index);
    if(package_range_downloader_ptrs_.size() <= index)
        package_range_downloader_ptrs_.resize(index+1);
    package_range_downloader_ptrs_[index] = package_range_downloader_;
    package_range_downloader_->start();
    connect(package_range_downloader_, SIGNAL(finished(std::shared_ptr<QByteArray>, int, int)), this, SLOT(on_one_package_downloaded(std::shared_ptr<QByteArray>, int, int)));
    ui->index_spin_box->setValue(ui->index_spin_box->value() + 1);
    if(ui->auto_acc_button->isChecked() && ui->end_spin_box->value() < total_length_){
        int end = ui->end_spin_box->value();
        int start = end + 1;
        end = end + 1024 * 1024 * 4 > total_length_ ? total_length_ : end + 1024 * 1024 * 4;
        ui->start_spin_box->setValue(start);
        ui->end_spin_box->setValue(end);
    }
}

void Widget::on_one_package_downloaded(std::shared_ptr<QByteArray> data, int index, int ret) {
    if(ret != 0){
        ui->status_textedit->append(QString("%1%2").arg("read error... ").arg(index));
        if(package_range_downloader_ptrs_[index] != nullptr)
            delete package_range_downloader_ptrs_[index];
        return;
    }
    ui->status_textedit->append(QString("%1%2%3%4").arg("size: ").arg(data->size()).arg(" index: ").arg(index));
    ui->status_textedit->append(QString(data->left(10)));
    fileBuffer_->write(data, index);
    if(package_range_downloader_ptrs_[index] != nullptr)
        delete package_range_downloader_ptrs_[index];
}
