#include "MyOrderWindow.h"
#include "DataInterface.h"
#include "ui_MyOrderWindow.h"

MyOrderWindow::MyOrderWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MyOrderWindow) {
    ui->setupUi(this);
}

MyOrderWindow::~MyOrderWindow() {
    delete ui;
}