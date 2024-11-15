#pragma once
#include <QSqlDatabase>

class DBInstance {
public:
    static QSqlDatabase& getInstance();
    ~DBInstance();
protected:
    DBInstance();
    class DBGarbo {
    public:
        ~DBGarbo() {
            if (instance != nullptr) {
                delete instance;
                instance = nullptr;
            }
        }
    };
    static DBGarbo dbGarbo;
    static DBInstance* instance;
    QSqlDatabase* db;
};