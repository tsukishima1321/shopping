#include "SellerApplyWindow.h"
#include "ui_SellerApplyWindow.h"
#include <QMessageBox>
#include "CurrentUser.h"
#include "DataInterface.h"

SellerApplyWindow::SellerApplyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SellerApplyWindow)
{
    ui->setupUi(this);
    User user = CurrentUser::getInstance()->getUser();
    if(user.isSeller){
        Seller seller = CurrentUser::getInstance()->getSeller();
        ui->lineEditRealName->setText(seller.realName);
        ui->lineEditIDNumber->setText(seller.realIdentityNumber);
        ui->lineEditPhone->setText(seller.phone);
    }
    connect(ui->buttonSubmitApply, &QPushButton::clicked, this, &SellerApplyWindow::submitApply);
}

void SellerApplyWindow::submitApply(){
    if(ui->lineEditPhone->text().isEmpty() || ui->lineEditRealName->text().isEmpty() || ui->lineEditIDNumber->text().isEmpty()){
        QMessageBox::warning(this, "提示", "请填写完整信息");
        return;
    }
    SellerApplySubmitStatus status = DataInterface::SubmitSellerApply(CurrentUser::getInstance()->getUser().id, ui->lineEditPhone->text(), ui->lineEditRealName->text(), ui->lineEditIDNumber->text());
    if(status == SellerApplySubmitStatus::Success){
        QMessageBox::information(this, "申请成功", "申请已提交，请耐心等待审核");
    } else if(status == SellerApplySubmitStatus::AlreadyApplied){
        QMessageBox::warning(this, "提示", "存在正在审核的申请，请耐心等待");
    } else {
        QMessageBox::warning(this, "申请失败", "申请失败");
    }
}

SellerApplyWindow::~SellerApplyWindow()
{
    delete ui;
}