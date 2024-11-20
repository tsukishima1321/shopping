#include "UserRegistWindow.h"
#include "DataInterface.h"
#include "ui_UserRegistWindow.h"
#include <QMessageBox>

UserRegistWindow::UserRegistWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::UserRegistWindow) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &UserRegistWindow::registButtonClicked);
}

UserRegistWindow::~UserRegistWindow() {
    delete ui;
}

void UserRegistWindow::registButtonClicked() {
    QString name = ui->lineEditName->text();
    QString password = ui->lineEditPassword->text();
    QString nickname = ui->lineEditNick->text();
    if (name.isEmpty() || password.isEmpty()) {
        return;
    }
    std::optional<ID_t> userId = DataInterface::UserRegist(name, password, nickname);
    if (userId.has_value()) {
        QMessageBox::information(this, "注册成功", "注册成功，返回登录界面");
        this->close();
    } else {
        QMessageBox::warning(this, "注册失败", "用户名已存在");
    }
}