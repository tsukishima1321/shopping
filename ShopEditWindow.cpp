#include "ShopEditWindow.h"
#include "DataInterface.h"
#include "GlobalConfig.h"
#include "GoodsDetailWindow.h"
#include "GoodsEditWindow.h"
#include "IconResources.h"
#include "ui_ShopEditWindow.h"
#include <QMessageBox>
#include <QGraphicsEffect>

ShopEditWindow::ShopEditWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::ShopEditWindow) {
    ui->setupUi(this);
    currentColumnCount = 3;
    hBoxLayout = new QHBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(hBoxLayout);
    ui->labelShopIcon->setPixmap(IconResources::getPixmaps()["store"]);

    connect(ui->checkBoxSelectAll, &QCheckBox::stateChanged, [this](int state) {
        for (auto &&form : previewList) {
            form->setChecked(state == Qt::Checked);
        }
    });

    connect(ui->buttonDeactive, &QPushButton::clicked, [this]() {
        for (auto &&form : previewList) {
            if (form->isChecked()) {
                if (!DataInterface::DeActiveGoods(form->getId())) {
                    QMessageBox::warning(this, "错误", "下架商品失败");
                }
            }
        }
        updateGoods();
    });

    connect(ui->buttonActive, &QPushButton::clicked, [this]() {
        for (auto &&form : previewList) {
            if (form->isChecked()) {
                if (!DataInterface::ActiveGoods(form->getId())) {
                    QMessageBox::warning(this, "错误", "上架商品失败");
                }
            }
        }
        updateGoods();
    });
}

void ShopEditWindow::updateGoods() {
    ui->checkBoxSelectAll->blockSignals(true);
    ui->checkBoxSelectAll->setChecked(false);
    ui->checkBoxSelectAll->blockSignals(false);
    for (auto &&form : previewList) {
        form->deleteLater();
    }
    previewList.clear();
    goodsList = DataInterface::getGoodsByShopId(shop.id);
    for (const Goods &goods : goodsList) {
        previewList.append(new GoodsPreviewForm);
        auto form = addGoodsItem(goods.image, goods.name, goods.price.toString(), goods.description, goods.id);
        form->enableCheck(true);
        if (goods.status == 1) {
            form->setDeactive();
        }
        connect(form, &GoodsPreviewForm::isClicked, this, &ShopEditWindow::openDetailMenu);
    }
    locateGoods();
}

void ShopEditWindow::setShopId(ID_t shopId) {
    shop = DataInterface::getShopById(shopId);
    goodsList = DataInterface::getGoodsByShopId(shopId);
    ui->labelShopName->setText(shop.name);
    updateGoods();
}

void ShopEditWindow::locateGoods() {
    for (auto &&form : previewList) {
        QVBoxLayout *columnMinHeight = nullptr;
        int minHeight = 999999;
        for (QVBoxLayout *column : vBoxLayouts) {
            int height = 0;
            for (int i = column->count() - 1; i >= 0; i--) {
                height += dynamic_cast<GoodsPreviewForm *>(column->itemAt(i)->widget())->getHeight();
            }
            if (height < minHeight) {
                // qDebug() << height;
                columnMinHeight = column;
                minHeight = height;
            }
        }
        if (columnMinHeight) {
            columnMinHeight->addWidget(form);
        }
    }
}

void ShopEditWindow::resizeEvent(QResizeEvent *event) {
    (void)event;
    int width = ui->scrollArea->width();
    if (ui->scrollArea->width() == 100) {
        currentColumnCount = 3;
    } else {
        currentColumnCount = width / 300;
    }
    for (QVBoxLayout *column : vBoxLayouts) {
        for (int i = column->count() - 1; i >= 0; i--) {
            column->removeItem(column->itemAt(i));
        }
    }
    QLayoutItem *column;
    while ((column = hBoxLayout->itemAt(0))) {
        hBoxLayout->removeItem(column);
        delete column;
    }
    vBoxLayouts.clear();
    for (int i = 0; i < currentColumnCount; i++) {
        QVBoxLayout *column = new QVBoxLayout();
        column->setAlignment(Qt::AlignTop);
        hBoxLayout->addLayout(column);
        vBoxLayouts.append(column);
    }
    locateGoods();
    QMainWindow::resizeEvent(event);
}

GoodsPreviewForm *ShopEditWindow::addGoodsItem(QString image, QString name, QString price, QString des, ID_t id) {
    GoodsPreviewForm *formToAdd = nullptr;
    for (auto &&form : previewList) {
        if (form->isAvailable()) {
            formToAdd = form;
            break;
        }
    }
    if (formToAdd) {
        formToAdd->setId(id);
        if (!image.isEmpty()) {
            QString imagePath = GlobalConfig::getInstance()->getStaticPath() + image;
            formToAdd->setImage(QPixmap(imagePath));
        }
        formToAdd->setText(name, price, des);
    }
    return formToAdd;
}

void ShopEditWindow::openDetailMenu(ID_t id) {
    GoodsEditWindow *detailWindow = new GoodsEditWindow();
    detailWindow->setGoodsId(id);
    detailWindow->show();
}

ShopEditWindow::~ShopEditWindow() {
    for (GoodsPreviewForm *form : previewList) {
        delete form;
    }
    if (hBoxLayout) {
        delete hBoxLayout;
    }
    delete ui;
}