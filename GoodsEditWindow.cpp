#include "GoodsEditWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "GlobalConfig.h"
#include "Iconresources.h"
#include "ui_GoodsEditWindow.h"
#include <QFileDialog>
#include <QMessageBox>

GoodsEditWindow::GoodsEditWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::GoodsEditWindow) {
    ui->setupUi(this);
    goods.id = 0;
    connect(ui->buttonSubmit, &QPushButton::clicked, this, &GoodsEditWindow::submit);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &GoodsEditWindow::cancel);
    connect(ui->buttonUploadImg, &QPushButton::clicked, this, &GoodsEditWindow::uploadImage);
}

void GoodsEditWindow::setGoodsId(ID_t id) {
    goods = DataInterface::getGoodsById(id);
    loadGoodsInfo();
}

void GoodsEditWindow::setShopId(ID_t id) {
    goods.shopId = id;
}

void GoodsEditWindow::loadGoodsInfo() {
    ui->lineEditName->setText(goods.name);
    ui->textEditDes->setPlainText(goods.description);
    ui->doubleSpinBox->setValue(static_cast<double>(goods.price));
    if (goods.image.isEmpty()) {
        ui->labelGoodsImage->setPixmap(IconResources::getPixmaps()["default-goods"]);
    } else {
        ui->labelGoodsImage->setPixmap(QPixmap(GlobalConfig::getInstance()->getStaticPath() + goods.image).scaled(ui->labelGoodsImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
}

void GoodsEditWindow::uploadImage() {
    QString staticPath = GlobalConfig::getInstance()->getStaticPath();
    QString imagePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) {
        return;
    }
    QString image = imagePath.right(imagePath.size() - imagePath.lastIndexOf('/') - 1);
    goods.image = image;
    QFile::copy(imagePath, staticPath + image);
    ui->labelGoodsImage->setPixmap(QPixmap(staticPath + imagePath).scaled(ui->labelGoodsImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

void GoodsEditWindow::submit() {
    UserPermission permission = DataInterface::getUserPermissionByUserId(CurrentUser::getInstance()->getUserId());
    if (!permission.allowAddGoods) {
        QMessageBox::warning(this, "错误", "你已被管理员禁止添加商品");
    }
    goods.name = ui->lineEditName->text();
    goods.description = ui->textEditDes->toPlainText();
    goods.price = Price(ui->doubleSpinBox->value());
    if (goods.id == 0) {
        std::optional<ID_t> id = DataInterface::AddGoods(goods);
        if (!id) {
            QMessageBox::warning(this, "错误", "添加商品失败");
        } else {
            QMessageBox::information(this, "成功", "添加商品成功");
            goods.id = *id;
            emit goodsChanged();
        }
    } else {
        if (!DataInterface::UpdateGoods(goods)) {
            QMessageBox::warning(this, "错误", "修改商品失败");
        } else {
            QMessageBox::information(this, "成功", "修改商品成功");
            emit goodsChanged();
        }
    }
}

void GoodsEditWindow::cancel() {
    if (goods.id == 0) {
        close();
    } else {
        goods = DataInterface::getGoodsById(goods.id);
        loadGoodsInfo();
    }
}

GoodsEditWindow::~GoodsEditWindow() {
    delete ui;
}