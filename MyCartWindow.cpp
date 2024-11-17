#include "MyCartWindow.h"
#include "CurrentUser.h"
#include "GoodsDetailWindow.h"
#include "ui_MyCartWindow.h"
#include <QDialog>
#include <QDialogButtonBox>

MyCartWindow::MyCartWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MyCartWindow) {
    ui->setupUi(this);
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->setAlignment(Qt::AlignTop);
    ui->scrollAreaWidgetContents->setLayout(vBoxLayout);

    updateGoods();
}

void MyCartWindow::updateGoods() {
    while (QLayoutItem *item = vBoxLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    goodsList = DataInterface::getGoodsInUserCart(CurrentUser::getInstance()->getUserId());
    for (auto &goods : goodsList) {
        auto *goodsWithQuantityForm = new GoodsWithQuantityForm();
        connect(goodsWithQuantityForm, &GoodsWithQuantityForm::quantityChanged, this, [this](ID_t id, int quantity) {
            DataInterface::UpdateGoodsQuantityInCart(CurrentUser::getInstance()->getUserId(), id, quantity);
            goodsList = DataInterface::getGoodsInUserCart(CurrentUser::getInstance()->getUserId());
            updateTotalPrice();
        });
        connect(goodsWithQuantityForm, &GoodsWithQuantityForm::isDeleted, this, [this](ID_t id) {
            QDialog dialog;
            QVBoxLayout layout(&dialog);
            QLabel label("确定要删除吗？");
            layout.addWidget(&label);
            QDialogButtonBox buttonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, Qt::Horizontal, &dialog);
            layout.addWidget(&buttonBox);
            QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
            if (dialog.exec() == QDialog::Accepted) {
                DataInterface::RemoveGoodsFromCart(CurrentUser::getInstance()->getUserId(), id);
                updateGoods();
            }
        });
        connect(goodsWithQuantityForm, &GoodsWithQuantityForm::isClicked, this, [this](ID_t id) {
            auto *goodsDetailWindow = new GoodsDetailWindow(id);
            goodsDetailWindow->show();
        });
        goodsWithQuantityForm->setGoods(goods.goods, goods.quantity);
        vBoxLayout->addWidget(goodsWithQuantityForm);
    }
    updateTotalPrice();
}

void MyCartWindow::updateTotalPrice() {
    Price totalPrice = Price();
    for (auto &goods : goodsList) {
        if(goods.goods.status == 1) {
            continue;
        }
        totalPrice = totalPrice + goods.totalPrice;
    }
    ui->labelTotalPrice->setText(totalPrice.toString());
}

MyCartWindow::~MyCartWindow() {
    delete ui;
}