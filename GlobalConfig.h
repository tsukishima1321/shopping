#pragma once
#include <QString>

struct DBConfig {
    QString host;
    QString port;
    QString user;
    QString password;
    QString database;
};

class GlobalConfig {

public:
    static GlobalConfig *getInstance();
    QString getStaticPath();
    DBConfig getDBConfig();
    ~GlobalConfig() = default;
protected:
    GlobalConfig();
    static GlobalConfig *instance;
    QString staticPath;
    DBConfig dbConfig;
};