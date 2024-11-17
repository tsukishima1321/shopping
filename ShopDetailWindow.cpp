#include "ShopDetailWindow.h"
#include "DataInterface.h"
#include "GlobalConfig.h"
#include "GoodsDetailWindow.h"
#include "Iconresources.h"
#include "ui_ShopDetailWindow.h"

ShopDetailWindow::ShopDetailWindow(ID_t shopId, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ShopDetailWindow),
        shop(DataInterface::getShopById(shopId)),
        goodsList(DataInterface::getGoodsByShopId(shop.id)),
        currentColumnCount(3) {
    ui->setupUi(this);
    hBoxLayout = new QHBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(hBoxLayout);
    ui->labelShopIcon->setPixmap(IconResources::getPixmaps()["store"]);
    ui->labelShopName->setText(shop.name);
    ui->labelShopDes->setText(shop.description);

    for (const Goods &goods : goodsList) {
        previewList.append(new GoodsPreviewForm);
        auto form = addGoodsItem(goods.image, goods.name, goods.price.toString(), goods.description, goods.id);
        connect(form, &GoodsPreviewForm::isClicked, this, &ShopDetailWindow::openDetailMenu);
    }
    locateGoods();
}

void ShopDetailWindow::locateGoods() {
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

GoodsPreviewForm *ShopDetailWindow::addGoodsItem(QString image, QString name, QString price, QString des, ID_t id) {
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

void ShopDetailWindow::resizeEvent(QResizeEvent *event) {
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

void ShopDetailWindow::openDetailMenu(ID_t id) {
    GoodsDetailWindow *detailWindow = new GoodsDetailWindow(id);
    detailWindow->show();
}

ShopDetailWindow::~ShopDetailWindow() {
    for (GoodsPreviewForm *form : previewList) {
        delete form;
    }
    if (hBoxLayout) {
        delete hBoxLayout;
    }
    delete ui;
}