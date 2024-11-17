#include "HandleOrderWindow.h"
#include "CurrentUser.h"
#include "DataInterface.h"
#include "OrderDetailView.h"
#include "ui_HandleOrderWindow.h"
#include <QHeaderView>
#include <QMessageBox>

HandleOrderWindow::HandleOrderWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::HandleOrderWindow) {
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &HandleOrderWindow::loadOrders);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, [this](int row, int column) {
        if (column != 3 || ui->tableWidget->item(row, 3)->text() != "未处理") {
            const Order &order = DataInterface::getOrderById(ui->tableWidget->item(row, 0)->text().toInt());
            OrderDetailView *orderDetailView = new OrderDetailView(order, this);
            orderDetailView->show();
        }else{
            int orderId = ui->tableWidget->item(row, 0)->text().toInt();
            if(QMessageBox::question(this, "确认", "是否接受该订单？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
                DataInterface::UpdateOrderStatus(orderId, 1);
            }else{
                DataInterface::UpdateOrderStatus(orderId, 2);
            }
            loadOrders();
        }
    });
    loadOrders();
}

void HandleOrderWindow::loadOrders() {
    ui->tableWidget->setRowCount(0);
    QVector<Order> orders;
    if (CurrentUser::getInstance()->isSeller()) {
        orders = DataInterface::getOrdersBySellerId(CurrentUser::getInstance()->getUserId());
        for (const auto &order : orders) {
            if (order.status == 0 || !ui->checkBox->isChecked()) {
                Price total;
                for (const auto &goods : order.goods) {
                    total = total + goods.totalPrice;
                }
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(order.orderId)));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(total.toString()));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(order.time.toString("yyyy-MM-dd hh:mm:ss")));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(order.status == 0 ? "未处理" : (order.status == 1 ? "已完成" : "已中止")));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(order.addressText + " " + order.receiverName + " " + order.receiverPhone));
            }
        }
    }
}

HandleOrderWindow::~HandleOrderWindow() {
    delete ui;
}