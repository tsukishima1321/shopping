#include "GlobalConfig.h"
#include <QSettings>
#include <QFileInfo>

GlobalConfig *GlobalConfig::instance = nullptr;

GlobalConfig *GlobalConfig::getInstance() {
    if (instance == nullptr) {
        instance = new GlobalConfig();
    }
    return instance;
}

GlobalConfig::GlobalConfig() {
    if (!QFileInfo("config.ini").isFile()) {
        QFile file("config.ini");
        file.open(QIODevice::Append);
        file.close();
        QSettings setting("config.ini", QSettings::IniFormat);
        setting.setValue("resource/staticPath", "D:/Homework/DB/static/");
        setting.setValue("database/host", "localhost");
        setting.setValue("database/port", "1433");
        setting.setValue("database/user", "sa");
        setting.setValue("database/password", "343542");
        setting.setValue("database/database", "shopping");
    }
    QSettings setting("config.ini", QSettings::IniFormat);
    staticPath = setting.value("resource/staticPath").toString();
    dbConfig.host = setting.value("database/host").toString();
    dbConfig.port = setting.value("database/port").toString();
    dbConfig.user = setting.value("database/user").toString();
    dbConfig.password = setting.value("database/password").toString();
    dbConfig.database = setting.value("database/database").toString();
}

QString GlobalConfig::getStaticPath() {
    return staticPath;
}

DBConfig GlobalConfig::getDBConfig() {
    return dbConfig;
}