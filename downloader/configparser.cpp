#include "configparser.h"
#include <QtDebug>

IDMConfigParser::IDMConfigParser(const QString &fileName)
    : fileName_(fileName),
      configs_(),
      file_(fileName)
{
    if(!file_.exists()){
        qDebug() << "file not exist: " << fileName_;
        return;
    }
    if(!file_.open(QIODevice::ReadOnly)){
        qDebug() << "file open error: " << fileName_;
    }
    parse();
}

void IDMConfigParser::parse()
{
    QByteArray buffer{};
    QString tmp{};
    while(true){
        buffer.clear();
        buffer = file_.readLine();
        if(buffer.isEmpty()) break;
        tmp = buffer;
        if(tmp.startsWith("<")){
            configs_.push_back(std::shared_ptr<ConfigContents>(new ConfigContents{}));
            qDebug() << "find one config element...";
        }else if(tmp.startsWith("http")){
            configs_.back()->url = tmp.trimmed();
            qDebug() << "url:" << configs_.back()->url;
        }else if(tmp.startsWith("referer:")){
            QString value = tmp.right(tmp.size() - QString("referer:").size()).trimmed();
            configs_.back()->headers.push_back(std::pair<QString, QString>("referer", value));
            qDebug() << "referer:" << value;
        }else if(tmp.startsWith("user-agent:")){
            QString value = tmp.right(tmp.size() - QString("user-agent:").size()).trimmed();
            configs_.back()->headers.push_back(std::pair<QString, QString>("user-agent", value));
            qDebug() << "user-agent:" << value;
        }else if(tmp.startsWith("cookie:")){
            QString value = tmp.right(tmp.size() - QString("cookie:").size()).trimmed();
            auto strs = value.split(" ");
            QList<QNetworkCookie> cookies;
            for(auto str : strs){
                cookies += QNetworkCookie::parseCookies(str.toUtf8());
            }
            configs_.back()->cookies = cookies;
//            qDebug() << "cookie:" << value;
            qDebug() << cookies.size();
            for(auto c : cookies){
                qDebug() << c.name() << c.value();
            }
        }else if(tmp.startsWith(">")){
            qDebug() << "parsed a config element..";
        }else{
            continue;
        }
    }
}


