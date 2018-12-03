#ifndef IDMCONFIGPARSER_H
#define IDMCONFIGPARSER_H

#include <QString>
#include <vector>
#include <memory>
#include <QFile>
#include <QList>
#include <QNetworkCookie>

struct ConfigContents{
    void clear(){
        url.clear();
        headers.clear();
        cookies.clear();
    }
    QString                                     url;
    std::vector<std::pair<QString, QString>>    headers;
    QList<QNetworkCookie>                       cookies;
};

class IDMConfigParser {
public:
    IDMConfigParser(const QString& fileName);
    const std::vector<std::shared_ptr<ConfigContents>>& getconfigs() const {return configs_;}
private:
    void parse();
private:
    QString                                             fileName_;
    std::vector<std::shared_ptr<ConfigContents>>        configs_;
    QFile                                               file_;
};

#endif // IDMCONFIGPARSER_H
