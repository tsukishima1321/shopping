#include "OrderDetailView.h"
#include "ui_OrderDetailView.h"
#include "GoodsWithQuantityForm.h"

OrderDetailView::OrderDetailView(const Order &order, QWidget *parent) :
        QMainWindow(parent), 
        ui(new Ui::OrderDetailView),
        order(order) {
    ui->setupUi(this);
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->setAlignment(Qt::AlignTop);
    ui->scrollAreaWidgetContents->setLayout(vBoxLayout);
    for(auto &goods : order.goods) {
        auto *goodsWithQuantityForm = new GoodsWithQuantityForm();
        goodsWithQuantityForm->setGoods(goods.goods, goods.quantity);
        goodsWithQuantityForm->setReadOnly(true);
        vBoxLayout->addWidget(goodsWithQuantityForm);
    }
}

OrderDetailView::~OrderDetailView() {
    while(QLayoutItem *item = vBoxLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    delete vBoxLayout;
    delete ui;
}