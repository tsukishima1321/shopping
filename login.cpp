#include "login.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "UserMainWindow.h"
#include "UserRegistWindow.h"
#include "iconresources.h"
#include "AdminMainWindow.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Login) {
    ui->setupUi(this);
    ui->toolButton->setIcon(IconResources::getIcons()["square-x"]);
    ui->lineEditName->setFocus();
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    ui->pushButton->setAutoDefault(false);
    ui->pushButton->setDefault(false);
    ui->buttonRegist->setAutoDefault(false);
    ui->buttonRegist->setDefault(false);
    connect(ui->lineEditPwd, &QLineEdit::returnPressed, this, &Login::pushButton_clicked);
    connect(ui->lineEditName, &QLineEdit::returnPressed, this, &Login::lineEditName_returnPressed);
    connect(ui->pushButton, &QPushButton::clicked, this, &Login::pushButton_clicked);
    connect(ui->toolButton, &QPushButton::clicked, this, [this]() { this->done(QDialog::Rejected); });
    connect(ui->buttonRegist, &QPushButton::clicked, this, &Login::registButton_clicked);
}

Login::~Login() {
    delete ui;
}

void Login::pushButton_clicked() {
    if (ui->checkBox->isChecked()) {
        std::optional<ID_t> adminID = DataInterface::AdminLogin(ui->lineEditName->text(), ui->lineEditPwd->text());
        if (adminID.has_value()) {
            QMessageBox::information(this, "登录成功", "管理员登录成功");
            AdminMainWindow *adminMainWindow = new AdminMainWindow();
            adminMainWindow->show();
            this->done(QDialog::Accepted);
        } else {
            QMessageBox::warning(this, "登录失败", "管理员用户名或密码错误");
        }
    } else {
        std::optional<ID_t> userId = DataInterface::UserLogin(ui->lineEditName->text(), ui->lineEditPwd->text());
        if (userId.has_value()) {
            User user = DataInterface::getUserById(userId.value());
            if (user.isSeller) {
                Seller seller = DataInterface::getSellerById(userId.value());
                CurrentUser::getInstance()->setSeller(seller);
            } else {
                CurrentUser::getInstance()->setUser(user);
            }
            UserMainWindow *userMainWindow = new UserMainWindow();
            userMainWindow->show();
            this->done(QDialog::Accepted);
        } else {
            QMessageBox::warning(this, "登录失败", "用户名或密码错误，或你已被管理员禁止登录");
        }
    }
}

void Login::registButton_clicked() {
    UserRegistWindow *userRegistWindow = new UserRegistWindow();
    userRegistWindow->show();
}

void Login::lineEditName_returnPressed() {
    ui->lineEditPwd->setFocus();
}
