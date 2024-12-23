#include "UserMainWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "EditUserWindow.h"
#include "HandleOrderWindow.h"
#include "IconResources.h"
#include "MyCartWindow.h"
#include "MyCollectionWindow.h"
#include "MyOrderWindow.h"
#include "SearchWindow.h"
#include "SellerApplyWindow.h"
#include "ShopEditWindow.h"
#include "ui_UserMainWindow.h"
#include <QMessageBox>

UserMainWindow::UserMainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::UserMainWindow) {
    ui->setupUi(this);
    vBoxLayout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(vBoxLayout);
    ui->labelUserName->setText(CurrentUser::getInstance()->getUser().nickname);
    ui->labelUserName->adjustSize();
    if (CurrentUser::getInstance()->getUser().defaultAddress == nullptr) {
        ui->labelDefaultAddress->setText("未设置地址信息");
    } else {
        ui->labelDefaultAddress->setText(CurrentUser::getInstance()->getUser().defaultAddress->addressText);
        ui->labelDefaultAddress->adjustSize();
    }
    if (CurrentUser::getInstance()->isSeller()) {
        ui->labelSellerName->setText(CurrentUser::getInstance()->getSeller().nickname);
        ui->labelSellerName->adjustSize();
        ui->labelSellerRealName->setText(CurrentUser::getInstance()->getSeller().realName);
        ui->labelSellerRealName->adjustSize();
        ui->labelSellerIDNumber->setText(CurrentUser::getInstance()->getSeller().realIdentityNumber);
        ui->labelSellerIDNumber->adjustSize();
        ui->buttonSellerRegist->hide();
        QVector<Shop> shops = DataInterface::getShopsBySellerId(CurrentUser::getInstance()->getSeller().id);
        for (auto &shop : shops) {
            if (shop.status != 1) {
                ShopPreviewForm *shopPreviewForm = new ShopPreviewForm;
                shopPreviewForm->setShop(shop);
                vBoxLayout->addWidget(shopPreviewForm);
            }
        }
    } else {
        ui->buttonHandleOrder->hide();
        ui->labelSellerName->hide();
        ui->labelSellerRealName->hide();
        ui->labelSellerIDNumber->hide();
        ui->scrollArea->setEnabled(false);
        ui->buttonAddShop->setEnabled(false);
        ui->buttonRefresh->setEnabled(false);
    }
    ui->buttonSearch->setIcon(IconResources::getIcons()["search"]);
    ui->buttonRefresh->setIcon(IconResources::getIcons()["refresh"]);
    ui->buttonRefreshCollection->setIcon(IconResources::getIcons()["refresh"]);

    MyCollectionWindow *myCollectionWindow = new MyCollectionWindow();
    ui->verticalLayout_7->addWidget(myCollectionWindow);

    connect(ui->buttonSearch, &QPushButton::clicked, this, &UserMainWindow::buttonSearchClicked);
    connect(ui->buttonStartShopping, &QPushButton::clicked, this, &UserMainWindow::buttonStartShoppingClicked);
    connect(ui->buttonAddShop, &QPushButton::clicked, this, &UserMainWindow::buttonAddShopClicked);
    connect(ui->buttonEditUser, &QPushButton::clicked, this, &UserMainWindow::buttonEditUserClicked);
    connect(ui->buttonMyOrder, &QPushButton::clicked, this, &UserMainWindow::buttonMyOrderClicked);
    connect(ui->buttonMyCart, &QPushButton::clicked, this, &UserMainWindow::buttonMyCartClicked);
    connect(ui->buttonRefresh, &QToolButton::clicked, this, &UserMainWindow::buttonRefreshClicked);
    connect(ui->buttonSellerRegist, &QPushButton::clicked, this, &UserMainWindow::buttonSellerApplyClicked);
    connect(ui->buttonHandleOrder, &QPushButton::clicked, this, [this]() {HandleOrderWindow* handleOrderWindow = new HandleOrderWindow;handleOrderWindow->show(); });
    connect(ui->buttonRefreshCollection, &QPushButton::clicked, myCollectionWindow, &MyCollectionWindow::updateCollection);
}

void UserMainWindow::buttonRefreshClicked() {
    while (QLayoutItem *item = vBoxLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    QVector<Shop> shops = DataInterface::getShopsBySellerId(CurrentUser::getInstance()->getSeller().id);
    for (auto &shop : shops) {
        if (shop.status != 1) {
            ShopPreviewForm *shopPreviewForm = new ShopPreviewForm;
            shopPreviewForm->setShop(shop);
            vBoxLayout->addWidget(shopPreviewForm);
        }
    }
}

void UserMainWindow::buttonSearchClicked() {
    QString searchName = ui->lineEdit->text();
    UserPermission userPermission = DataInterface::getUserPermissionByUserId(CurrentUser::getInstance()->getUserId());
    if (userPermission.allowShopping == 0) {
        QMessageBox::information(this, "提示", "你已被管理员禁止购物");
        return;
    }
    SearchWindow *searchWindow = new SearchWindow(searchName);
    searchWindow->show();
}

void UserMainWindow::buttonStartShoppingClicked() {
    UserPermission userPermission = DataInterface::getUserPermissionByUserId(CurrentUser::getInstance()->getUserId());
    if (userPermission.allowShopping == 0) {
        QMessageBox::information(this, "提示", "你已被管理员禁止购物");
        return;
    }
    SearchWindow *searchWindow = new SearchWindow();
    searchWindow->show();
}

void UserMainWindow::buttonAddShopClicked() {
    UserPermission userPermission = DataInterface::getUserPermissionByUserId(CurrentUser::getInstance()->getUserId());
    if (userPermission.allowAddShop == 0) {
        QMessageBox::information(this, "提示", "你已被管理员禁止新建店铺");
        return;
    }
    ShopEditWindow *shopEditWindow = new ShopEditWindow();
    shopEditWindow->show();
}

void UserMainWindow::buttonEditUserClicked() {
    EditUserWindow *editUserWindow = new EditUserWindow();
    connect(editUserWindow, &EditUserWindow::userChanged, this, &UserMainWindow::refreshUser);
    editUserWindow->show();
}

void UserMainWindow::refreshUser() {
    ui->labelUserName->setText(CurrentUser::getInstance()->getUser().nickname);
    ui->labelUserName->adjustSize();
    if (CurrentUser::getInstance()->getUser().defaultAddress == nullptr) {
        ui->labelDefaultAddress->setText("未设置地址信息");
    } else {
        ui->labelDefaultAddress->setText(CurrentUser::getInstance()->getUser().defaultAddress->addressText);
        ui->labelDefaultAddress->adjustSize();
    }
    if (CurrentUser::getInstance()->isSeller()) {
        ui->labelSellerName->setText(CurrentUser::getInstance()->getSeller().nickname);
        ui->labelSellerName->adjustSize();
        ui->labelSellerRealName->setText(CurrentUser::getInstance()->getSeller().realName);
        ui->labelSellerRealName->adjustSize();
        ui->labelSellerIDNumber->setText(CurrentUser::getInstance()->getSeller().realIdentityNumber);
        ui->labelSellerIDNumber->adjustSize();
    }
}

void UserMainWindow::buttonMyOrderClicked() {
    MyOrderWindow *myOrderWindow = new MyOrderWindow();
    myOrderWindow->show();
}

void UserMainWindow::buttonMyCartClicked() {
    MyCartWindow *myCartWindow = new MyCartWindow();
    myCartWindow->show();
}

void UserMainWindow::buttonSellerApplyClicked() {
    SellerApplyWindow *sellerApplyWindow = new SellerApplyWindow();
    sellerApplyWindow->show();
}

UserMainWindow::~UserMainWindow() {
    delete ui;
}