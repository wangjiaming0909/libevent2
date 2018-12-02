#include "filebuffer.h"
#include <QtDebug>


FileBuffer::FileBuffer(const QString &fileName, int sequence_count)
    :file_(fileName), stream_(&file_), sequence_count_(sequence_count), buffer_()
{
    file_.open(QIODevice::ReadWrite);
}

void FileBuffer::write(std::shared_ptr<QByteArray> data, int index) {
    if(index == -1) {
        do_write(*data, -1);
        return;
    }
    buffer_.insert(std::pair<int, std::shared_ptr<QByteArray>>(index, data));
    while(!buffer_.empty() && (buffer_.begin()->first == sequence_)){
        do_write(*buffer_.begin()->second, sequence_);
        if(sequence_ == sequence_count_){
            file_.close();
            qDebug() << "file closed...";
        }
        sequence_++;
        buffer_.erase(buffer_.begin());
    }
}

void FileBuffer::do_write(const QByteArray &data, int index) {
    qDebug() << index << " writing bytes: " << data.size();
    file_.write(data.constData(), data.size());
    file_.flush();
}
