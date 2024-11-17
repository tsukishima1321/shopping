#include "GoodsWithQuantityForm.h"
#include "DataInterface.h"
#include "GlobalConfig.h"
#include "IconResources.h"
#include "ui_GoodsWithQuantityForm.h"
#include <QGraphicsDropShadowEffect>

GoodsWithQuantityForm::GoodsWithQuantityForm(QWidget *parent) :
        QWidget(parent), ui(new Ui::GoodsWithQuantityForm) {
    active = true;
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
    if (goods.status == 1) {
        active = false;
        ui->labelGoodsName->setText("商品已下架");
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(10);
        effect->setColor(QColor(255, 0, 0, 255));
        effect->setOffset(0, 0);
        this->setGraphicsEffect(effect);
        ui->spinBox->setEnabled(false);
    }
}

ID_t GoodsWithQuantityForm::getId() const {
    return id;
}

int GoodsWithQuantityForm::getQuantity() {
    return ui->spinBox->value();
}

void GoodsWithQuantityForm::mouseDoubleClickEvent(QMouseEvent *event) {
    (void)event;
    emit isClicked(id);
}

GoodsWithQuantityForm::~GoodsWithQuantityForm() {
    delete ui;
}