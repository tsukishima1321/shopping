#include "MyOrderWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "OrderDetailView.h"
#include "ui_MyOrderWindow.h"

MyOrderWindow::MyOrderWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MyOrderWindow) {
    ui->setupUi(this);
    orders = DataInterface::getOrdersByUserId(CurrentUser::getInstance()->getUserId());
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    updateOrderView();
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, [this](int row, int column) {
        (void) column;
        const Order& order = orders[row];
        OrderDetailView *orderDetailView = new OrderDetailView(order,this);
        orderDetailView->show();
    });
}

MyOrderWindow::~MyOrderWindow() {
    delete ui;
}

void MyOrderWindow::updateOrderView() {
    ui->tableWidget->setRowCount(orders.size());
    for (int i = 0; i < orders.size(); i++) {
        Order &order = orders[i];
        Price totalPrice;
        for (GoodsWithQuantity &goods : order.goods) {
            totalPrice = totalPrice + goods.totalPrice;
        }
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(order.orderId)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(order.shopName));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(totalPrice.toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(order.time.toString()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(order.status == 0 ? "未处理" : (order.status == 1 ? "已完成" : "已中止")));
    }
}