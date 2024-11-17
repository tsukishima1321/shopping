#include "login.h"
#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMainWindow>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString path = QDir::temp().absoluteFilePath("SingleApp_shopping.lock");
    QLockFile lockFile(path);

    bool isLock = lockFile.isLocked();
    (void)isLock;
    if (!lockFile.tryLock(100)) {
        QMessageBox::warning(nullptr, "提示", "程序已在运行中");
        return 0;
    }

    Login window;
    window.show();
    return app.exec();
}