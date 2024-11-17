#include "ShopPreviewForm.h"
#include "DataInterface.h"
#include "IconResources.h"
#include "ShopEditWindow.h"
#include "ui_ShopPreviewForm.h"

ShopPreviewForm::ShopPreviewForm(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ShopPreviewForm) {
    ui->setupUi(this);
    ui->labelIcon->setPixmap(IconResources::getPixmaps()["store"]);
}

ShopPreviewForm::~ShopPreviewForm() {
    delete ui;
}

void ShopPreviewForm::setShop(Shop shop) {
    ui->labelName->setText(shop.name);
    ui->labelName->adjustSize();
    ui->labelDescription->setText(shop.description);
    ui->labelDescription->adjustSize();
    this->shop = shop;
}

void ShopPreviewForm::mouseDoubleClickEvent(QMouseEvent *event) {
    (void)event;
    ShopEditWindow *shopWindow = new ShopEditWindow();
    shopWindow->setShopId(this->shop.id);
    shopWindow->show();
}