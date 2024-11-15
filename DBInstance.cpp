#include "DBInstance.h"
#include "GlobalConfig.h"
#include <QMessageBox>
#include <QSqlError>

DBInstance *DBInstance::instance = nullptr;

QSqlDatabase &DBInstance::getInstance() {
    if (instance == nullptr) {
        instance = new DBInstance();
    }
    return *instance->db;
}

DBInstance::DBInstance() {
    // 需在Windows控制面板中配置ODBC数据源
    // 参考：https://blog.csdn.net/Mr_Blackk/article/details/120907783
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QODBC"));
    DBConfig config = GlobalConfig::getInstance()->getDBConfig();
    db->setHostName(config.host);
    db->setPort(config.port.toInt());
    db->setUserName(config.user);
    db->setPassword(config.password);
    db->setDatabaseName(config.database);
    db->open();
    if (!db->isOpen()) {
        QMessageBox::warning(nullptr, "提示", db->lastError().text() + "\n数据库连接失败，请检查配置文件");
        throw "数据库连接失败";
    }
}

DBInstance::~DBInstance() {
    db->close();
    delete db;
    db = nullptr;
}