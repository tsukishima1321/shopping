#include "login.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Login window;
    window.show();
    return app.exec();
}