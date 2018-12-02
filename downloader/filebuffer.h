#ifndef FILEBUFFER_H
#define FILEBUFFER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <map>
#include <memory>

class FileBuffer {
public:
    FileBuffer(const QString& fileName, int sequence_count);
    void write(std::shared_ptr<QByteArray>, int);
private:
    void do_write(const QByteArray& data, int);
private:
    QString     fileName_;
    QFile       file_;
    QTextStream stream_;
    int         sequence_count_;
    int         sequence_ = 0;
    std::map<int, std::shared_ptr<QByteArray>> buffer_;
};

#endif // FILEBUFFER_H
