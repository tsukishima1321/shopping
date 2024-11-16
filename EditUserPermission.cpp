#include "EditUserPermission.h"
#include "DataInterface.h"
#include "ui_EditUserPermission.h"
#include <QMessageBox>

EditUserPermission::EditUserPermission(ID_t userID, QWidget *parent) :
        QWidget(parent),
        userId(userID),
        ui(new Ui::EditUserPermission) {
    ui->setupUi(this);
    ui->labelUserID->setText(QString::number(userID));
    refresh();
    connect(ui->buttonSubmit, &QPushButton::clicked, this, &EditUserPermission::submit);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &EditUserPermission::cancel);
    connect(ui->buttonBlockAll, &QPushButton::clicked, this, &EditUserPermission::blockAll);
}

void EditUserPermission::refresh() {
    UserPermission userPermission = DataInterface::getUserPermissionByUserId(userId);
    ui->checkBoxAllowLogin->setChecked(userPermission.allowLogin);
    ui->checkBoxAllowShopping->setChecked(userPermission.allowShopping);
    ui->checkBoxAllowComment->setChecked(userPermission.allowComment);
    ui->checkBoxAllowAddShop->setChecked(userPermission.allowAddShop);
    ui->checkBoxAllowAddGoods->setChecked(userPermission.allowAddGoods);
    ui->checkBoxAllHandleOrder->setChecked(userPermission.allowHandleOrder);
}

void EditUserPermission::submit() {
    UserPermission userPermission;
    userPermission.allowLogin = ui->checkBoxAllowLogin->isChecked();
    userPermission.allowShopping = ui->checkBoxAllowShopping->isChecked();
    userPermission.allowComment = ui->checkBoxAllowComment->isChecked();
    userPermission.allowAddShop = ui->checkBoxAllowAddShop->isChecked();
    userPermission.allowAddGoods = ui->checkBoxAllowAddGoods->isChecked();
    userPermission.allowHandleOrder = ui->checkBoxAllHandleOrder->isChecked();
    if (DataInterface::setUserPermissionByUserId(userId, userPermission)) {
        QMessageBox::information(this, "成功", "修改成功");
    } else {
        QMessageBox::warning(this, "错误", "修改失败");
    }
    refresh();
}

void EditUserPermission::cancel() {
    refresh();
}

void EditUserPermission::blockAll() {
    ui->checkBoxAllowLogin->setChecked(false);
    ui->checkBoxAllowShopping->setChecked(false);
    ui->checkBoxAllowComment->setChecked(false);
    ui->checkBoxAllowAddShop->setChecked(false);
    ui->checkBoxAllowAddGoods->setChecked(false);
    ui->checkBoxAllHandleOrder->setChecked(false);
}

EditUserPermission::~EditUserPermission() {
    delete ui;
}