#include "EditUserWindow.h"
#include "ui_EditUserWindow.h"

EditUserWindow::EditUserWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::EditUserWindow) {
    ui->setupUi(this);
}

EditUserWindow::~EditUserWindow() {
    delete ui;
}