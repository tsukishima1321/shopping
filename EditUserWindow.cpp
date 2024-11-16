#include "EditUserWindow.h"
#include "CurrentUser.h"
#include "Iconresources.h"
#include "SellerApplyWindow.h"
#include "ui_EditUserWindow.h"
#include <QMessageBox>
#include <QInputDialog>

DeleteButton::DeleteButton(ID_t addressID, QWidget *parent) :
        QPushButton(parent),
        addressId(addressID) {
    setIcon(IconResources::getIcons()["trash"]);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(32, 32);
    connect(this, &DeleteButton::clicked, [this] {
        if (DataInterface::DeleteAddress(this->addressId)) {
            emit addressDeleted();
        } else {
            QMessageBox::warning(this, "删除失败", "删除失败");
        }
    });
}

DeleteButton::~DeleteButton() {}

SetDefaultButton::SetDefaultButton(ID_t addressID, QWidget *parent) :
        QPushButton(parent),
        addressId(addressID) {
    setIcon(IconResources::getIcons()["square-check"]);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(32, 32);
    connect(this, &SetDefaultButton::clicked, [this] {
        if (DataInterface::SetDefaultAddress(this->addressId, CurrentUser::getInstance()->getUserId())) {
            emit defaultAddressChanged();
        } else {
            QMessageBox::warning(this, "设置失败", "设置失败");
        }
    });
}

SetDefaultButton::~SetDefaultButton() {}

EditUserWindow::EditUserWindow(QWidget *parent) :
        QMainWindow(parent),
        user(CurrentUser::getInstance()->getUser()),
        ui(new Ui::EditUserWindow) {
    ui->setupUi(this);
    ui->buttonNewAddress->setIcon(IconResources::getIcons()["new-address"]);
    ui->lineEditName->setText(user.name);
    if(user.isSeller){
        Seller seller = CurrentUser::getInstance()->getSeller();
        ui->lineEditRealName->setText(seller.realName);
        ui->lineEditIDNumber->setText(seller.realIdentityNumber);
        ui->lineEditPhone->setText(seller.phone);
    }
    ui->tableWidgetAddress->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidgetAddress->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidgetAddress->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidgetAddress->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidgetAddress->setColumnWidth(3, 32);
    ui->tableWidgetAddress->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableWidgetAddress->setColumnWidth(4, 32);
    refreshAddressTable();
    connect(ui->buttonSubmit, &QPushButton::clicked, this, &EditUserWindow::submitChanges);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &EditUserWindow::cancelChanges);
    connect(ui->buttonNewAddress, &QToolButton::clicked, this, &EditUserWindow::newAddress);
    connect(ui->buttonSellerApply, &QPushButton::clicked, this, &EditUserWindow::sellerApply);
}

void EditUserWindow::refreshAddressTable() {
    user = CurrentUser::getInstance()->getUser();
    ui->tableWidgetAddress->clearContents();
    ui->tableWidgetAddress->setRowCount(0);
    for (Address &address : user.addresses) {
        if (address.id == user.defaultAddress->id) {
            ui->labelDefaultAddress->setText(address.addressText);
        }
        ui->tableWidgetAddress->insertRow(ui->tableWidgetAddress->rowCount());
        int row = ui->tableWidgetAddress->rowCount() - 1;
        ui->tableWidgetAddress->setItem(row, 0, new QTableWidgetItem(address.addressText));
        ui->tableWidgetAddress->setItem(row, 1, new QTableWidgetItem(address.receiverName));
        ui->tableWidgetAddress->setItem(row, 2, new QTableWidgetItem(address.receiverPhone));
        DeleteButton *deleteButton = new DeleteButton(address.id, ui->tableWidgetAddress);
        connect(deleteButton, &DeleteButton::addressDeleted, this, &EditUserWindow::refreshAddressTable);
        connect(deleteButton, &DeleteButton::addressDeleted, this, &EditUserWindow::userChanged);
        ui->tableWidgetAddress->setCellWidget(row, 3, deleteButton);
        SetDefaultButton *setDefaultButton = new SetDefaultButton(address.id, ui->tableWidgetAddress);
        connect(setDefaultButton, &SetDefaultButton::defaultAddressChanged, this, &EditUserWindow::refreshAddressTable);
        connect(setDefaultButton, &SetDefaultButton::defaultAddressChanged, this, &EditUserWindow::userChanged);
        ui->tableWidgetAddress->setCellWidget(row, 4, setDefaultButton);
    }
}

void EditUserWindow::newAddress() {
    QString addressText = QInputDialog::getText(this, "输入地址", "请输入地址");
    QString receiverName = QInputDialog::getText(this, "输入收件人姓名", "请输入收件人姓名");
    QString receiverPhone = QInputDialog::getText(this, "输入收件人电话", "请输入收件人电话");
    std::optional<ID_t> id = DataInterface::AddAddress(addressText, receiverName, receiverPhone, CurrentUser::getInstance()->getUserId());
    if (id.has_value()) {
        refreshAddressTable();
        emit userChanged();
    } else {
        QMessageBox::warning(this, "添加失败", "添加失败");
    }
}

EditUserWindow::~EditUserWindow() {
    delete ui;
}

void EditUserWindow::submitChanges() {
    User tempUser = user;
    tempUser.name = ui->lineEditName->text();
    if (ui->lineEditPassword->text() != "") {
        tempUser.password = ui->lineEditPassword->text();
    }
    std::optional<ID_t> id = DataInterface::UpdateUser(tempUser);
    if (id.has_value()) {
        user = CurrentUser::getInstance()->getUser();
        QMessageBox::information(this, "修改成功", "修改成功");
        emit userChanged();
    } else {
        QMessageBox::warning(this, "修改失败", "修改失败，用户名已存在");
    }
}

void EditUserWindow::cancelChanges() {
    ui->lineEditName->setText(user.name);
    ui->lineEditPassword->setText("");
}

void EditUserWindow::sellerApply(){
    SellerApplyWindow *sellerApplyWindow = new SellerApplyWindow();
    sellerApplyWindow->show();
}