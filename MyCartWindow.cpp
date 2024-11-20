#include "MyCartWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "GoodsDetailWindow.h"
#include "ui_MyCartWindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>

MyCartWindow::MyCartWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MyCartWindow) {
    ui->setupUi(this);
    addresses = DataInterface::getAddressesByUserId(CurrentUser::getInstance()->getUserId());
    int i = 0;
    for (auto &address : addresses) {
        ui->comboBox->addItem(address.addressText + " " + address.receiverName + " " + address.receiverPhone);
        if (address.id == CurrentUser::getInstance()->getUser().defaultAddress->id) {
            ui->comboBox->setCurrentIndex(i);
        }
        i++;
    }
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->setAlignment(Qt::AlignTop);
    ui->scrollAreaWidgetContents->setLayout(vBoxLayout);
    connect(ui->buttonSubmitOrder, &QPushButton::clicked, this, &MyCartWindow::subMitOrder);
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
        if (goods.goods.status == 1) {
            continue;
        }
        totalPrice = totalPrice + goods.totalPrice;
    }
    ui->labelTotalPrice->setText(totalPrice.toString());
}

MyCartWindow::~MyCartWindow() {
    delete ui;
}

void MyCartWindow::subMitOrder() {
    if(ui->comboBox->currentText().isEmpty()) {
        QMessageBox::warning(nullptr, "错误", "请选择有效的地址");
        return;
    }
    QDialog dialog;
    QVBoxLayout layout(&dialog);
    QLabel label("确定要提交订单吗？");
    layout.addWidget(&label);
    QDialogButtonBox buttonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, Qt::Horizontal, &dialog);
    layout.addWidget(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (!DataInterface::SubmitOrder(CurrentUser::getInstance()->getUserId(), addresses[ui->comboBox->currentIndex()].id)) {
        QMessageBox::warning(nullptr, "错误", "提交订单失败");
    }
    updateGoods();
}