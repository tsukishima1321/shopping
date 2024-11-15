#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Create a window
    QMainWindow window;
    window.resize(320, 240);
    window.show();
    return app.exec();
}