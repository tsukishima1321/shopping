#include "GoodsWithQuantityForm.h"
#include "GlobalConfig.h"
#include "IconResources.h"
#include "ui_GoodsWithQuantityForm.h"

GoodsWithQuantityForm::GoodsWithQuantityForm(QWidget *parent) :
        QWidget(parent), ui(new Ui::GoodsWithQuantityForm) {
    ui->setupUi(this);
    ui->labelImage->setPixmap(IconResources::getPixmaps()["default-goods"].scaled(ui->labelImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    ui->buttonDelete->setIcon(IconResources::getIcons()["trash"]);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
        ui->labelTotalPrice->setText((price * value).toString());
        emit quantityChanged(id, value);
    });
    connect(ui->buttonDelete, &QToolButton::clicked, [this]() {
        emit isDeleted(id);
    });
}

void GoodsWithQuantityForm::setGoods(const Goods &goods, int quantity) {
    ui->labelGoodsName->setText(goods.name);
    ui->labelShopName->setText(goods.shopName);
    ui->spinBox->blockSignals(true);
    ui->spinBox->setValue(quantity);
    ui->spinBox->blockSignals(false);
    if (!goods.image.isEmpty()) {
        QString imgPath = GlobalConfig::getInstance()->getStaticPath() + goods.image;
        ui->labelImage->setPixmap(QPixmap(imgPath).scaled(Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        ui->labelImage->setPixmap(IconResources::getPixmaps()["default-goods"].scaled(ui->labelImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    id = goods.id;
    price = goods.price;
    ui->labelTotalPrice->setText((price * quantity).toString());
}

ID_t GoodsWithQuantityForm::getId() const {
    return id;
}

int GoodsWithQuantityForm::getQuantity() {
    return ui->spinBox->value();
}

void GoodsWithQuantityForm::mouseDoubleClickEvent(QMouseEvent *event) {
    (void) event;
    emit isClicked(id);
}

GoodsWithQuantityForm::~GoodsWithQuantityForm() {
    delete ui;
}