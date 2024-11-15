#include "UserMainWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "IconResources.h"
#include "EditUserWindow.h"
#include "ui_UserMainWindow.h"

UserMainWindow::UserMainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::UserMainWindow) {
    ui->setupUi(this);
    vBoxLayout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(vBoxLayout);
    ui->labelUserName->setText(CurrentUser::getInstance()->getUser().name);
    ui->labelUserName->adjustSize();
    if(CurrentUser::getInstance()->getUser().defaultAddress == nullptr){
        ui->labelDefaultAddress->setText("未设置地址信息");
    } else {
        ui->labelDefaultAddress->setText(CurrentUser::getInstance()->getUser().defaultAddress->addressText);
        ui->labelDefaultAddress->adjustSize();
    }
    if (CurrentUser::getInstance()->isSeller()) {
        ui->labelSellerName->setText(CurrentUser::getInstance()->getSeller().name);
        ui->labelSellerName->adjustSize();
        ui->labelSellerRealName->setText(CurrentUser::getInstance()->getSeller().realName);
        ui->labelSellerRealName->adjustSize();
        ui->labelSellerIDNumber->setText(CurrentUser::getInstance()->getSeller().realIdentityNumber);
        ui->labelSellerIDNumber->adjustSize();
        ui->buttonSellerRegist->hide();
        QVector<Shop> shops = DataInterface::getShopsBySellerId(CurrentUser::getInstance()->getSeller().id);
        for (auto &shop : shops) {
            ShopPreviewForm *shopPreviewForm = new ShopPreviewForm;
            shopPreviewForm->setShop(shop);
            vBoxLayout->addWidget(shopPreviewForm);
        }
    } else {
        ui->labelSellerName->hide();
        ui->labelSellerRealName->hide();
        ui->labelSellerIDNumber->hide();
        ui->scrollArea->setEnabled(false);
        ui->buttonAddShop->setEnabled(false);
        ui->buttonRefresh->setEnabled(false);
    }
    ui->buttonSearch->setIcon(IconResources::getIcons()["search"]);
    ui->buttonRefresh->setIcon(IconResources::getIcons()["refresh"]);

    connect(ui->buttonSearch, &QPushButton::clicked, this, &UserMainWindow::buttonSearchClicked);
    connect(ui->buttonStartShopping, &QPushButton::clicked, this, &UserMainWindow::buttonStartShoppingClicked);
    connect(ui->buttonAddShop, &QPushButton::clicked, this, &UserMainWindow::buttonAddShopClicked);
    connect(ui->buttonEditUser, &QPushButton::clicked, this, &UserMainWindow::buttonEditUserClicked);
    connect(ui->buttonMyCollect, &QPushButton::clicked, this, &UserMainWindow::buttonMyCollectionClicked);
    connect(ui->buttonMyOrder, &QPushButton::clicked, this, &UserMainWindow::buttonMyOrderClicked);
    connect(ui->buttonMyCart, &QPushButton::clicked, this, &UserMainWindow::buttonMyCartClicked);
    connect(ui->buttonRefresh, &QToolButton::clicked, this, &UserMainWindow::buttonRefreshClicked);
}

void UserMainWindow::buttonRefreshClicked() {
    while(QLayoutItem *item = vBoxLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    QVector<Shop> shops = DataInterface::getShopsBySellerId(CurrentUser::getInstance()->getSeller().id);
    for (auto &shop : shops) {
        ShopPreviewForm *shopPreviewForm = new ShopPreviewForm;
        shopPreviewForm->setShop(shop);
        vBoxLayout->addWidget(shopPreviewForm);
    }
}

void UserMainWindow::buttonSearchClicked() {
    QString searchName = ui->lineEdit->text();
    /*
    SearchWindow *searchWindow = new SearchWindow(searchName);
    searchWindow->show();
    */
}

void UserMainWindow::buttonStartShoppingClicked() {
    /*
    SearchWindow *SearchWindow = new SearchWindow();
    searchWindow->show();
    */
}

void UserMainWindow::buttonAddShopClicked() {
    
}

void UserMainWindow::buttonEditUserClicked() {
    EditUserWindow *editUserWindow = new EditUserWindow();
    editUserWindow->show();
}

void UserMainWindow::buttonMyCollectionClicked() {
    /*
    MyCollectionWindow *myCollectionWindow = new MyCollectionWindow();
    myCollectionWindow->show();
    */
}

void UserMainWindow::buttonMyOrderClicked() {
    /*
    MyOrderWindow *myOrderWindow = new MyOrderWindow();
    myOrderWindow->show();
    */
}

void UserMainWindow::buttonMyCartClicked() {
    /*
    MyCartWindow *myCartWindow = new MyCartWindow();
    myCartWindow->show();
    */
}

UserMainWindow::~UserMainWindow() {
    delete ui;
}