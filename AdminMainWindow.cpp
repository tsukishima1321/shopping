#include "AdminMainWindow.h"
#include "DataInterface.h"
#include "EditUserPermission.h"
#include "ui_AdminMainWindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>

AdminMainWindow::AdminMainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::AdminMainWindow) {
    ui->setupUi(this);
    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetUser->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidgetSeller->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetSeller->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetSeller->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidgetShop->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetShop->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetShop->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidgetShop->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidgetGoods->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetGoods->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetGoods->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidgetGoods->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidgetApply->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetApply->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetApply->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidgetApply->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    refreshUser();
    refreshSeller();
    refreshShop();
    refreshGoods();
    refreshApply();

    connect(ui->tableWidgetUser, &QTableWidget::cellDoubleClicked, this, [this](int row, int column) {
        (void)column;
        ID_t userId = ui->tableWidgetUser->item(row, 0)->text().toUInt();
        EditUserPermission *editUserPermission = new EditUserPermission(userId, nullptr);
        editUserPermission->show();
    });

    connect(ui->tableWidgetShop, &QTableWidget::cellDoubleClicked, this, [this](int row, int column) {
        if (column == 3) {
            ID_t shopId = ui->tableWidgetShop->item(row, 0)->text().toUInt();
            if (ui->tableWidgetShop->item(row, 3)->text() == "正常") {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "关闭店铺", "确定关闭该店铺吗？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    closeShop(shopId);
                }
            } else {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "开启店铺", "确定开启该店铺吗？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    openShop(shopId);
                }
            }
        }
    });

    connect(ui->tableWidgetGoods, &QTableWidget::cellDoubleClicked, this, [this](int row, int column) {
        if (column == 4) {
            ID_t goodsId = ui->tableWidgetGoods->item(row, 0)->text().toUInt();
            if (ui->tableWidgetGoods->item(row, 4)->text() == "正常") {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "下架商品", "确定下架该商品吗？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    deActiveGoods(goodsId);
                }
            } else {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "上架商品", "确定上架该商品吗？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    activeGoods(goodsId);
                }
            }
        }
    });

    connect(ui->tableWidgetApply, &QTableWidget::cellDoubleClicked, this, [this](int row, int column) {
        if (column == 6) {
            ID_t applyId = ui->tableWidgetApply->item(row, 0)->text().toUInt();
            if (ui->tableWidgetApply->item(row, 6)->text() == "待审核") {
                QDialog dialog;
                dialog.setWindowTitle("审核申请");
                QVBoxLayout layout(&dialog);
                layout.addWidget(new QLabel("审核结果"));
                QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort, Qt::Horizontal, &dialog);
                layout.addWidget(&buttonBox);
                connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
                connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
                dialog.exec();
                if (dialog.result() == QDialog::Accepted) {
                    acceptApply(applyId);
                } else {
                    rejectApply(applyId);
                }
            }
        }
    });
}

void AdminMainWindow::closeShop(ID_t shopId) {
    if (DataInterface::CloseShop(shopId)) {
        refreshShop();
    } else {
        QMessageBox::warning(this, "错误", "关闭店铺失败");
    }
}

void AdminMainWindow::openShop(ID_t shopId) {
    if (DataInterface::OpenShop(shopId)) {
        refreshShop();
    } else {
        QMessageBox::warning(this, "错误", "开启店铺失败");
    }
}

void AdminMainWindow::deActiveGoods(ID_t goodsId) {
    if (DataInterface::DeActiveGoods(goodsId)) {
        refreshGoods();
    } else {
        QMessageBox::warning(this, "错误", "下架商品失败");
    }
}

void AdminMainWindow::activeGoods(ID_t goodsId) {
    if (DataInterface::ActiveGoods(goodsId)) {
        refreshGoods();
    } else {
        QMessageBox::warning(this, "错误", "上架商品失败");
    }
}

void AdminMainWindow::acceptApply(ID_t applyId) {
    if (DataInterface::AdminAcceptApply(applyId)) {
        refreshApply();
    } else {
        QMessageBox::warning(this, "错误", "审核申请失败");
    }
}

void AdminMainWindow::rejectApply(ID_t applyId) {
    if (DataInterface::AdminRejectApply(applyId)) {
        refreshApply();
    } else {
        QMessageBox::warning(this, "错误", "审核申请失败");
    }
}

void AdminMainWindow::refreshUser() {
    ui->tableWidgetUser->clearContents();
    ui->tableWidgetUser->setRowCount(0);
    auto users = DataInterface::AdminGetAllUsers();
    for (const auto &user : users) {
        int row = ui->tableWidgetUser->rowCount();
        ui->tableWidgetUser->insertRow(row);
        ui->tableWidgetUser->setItem(row, 0, new QTableWidgetItem(QString::number(user.user.id)));
        ui->tableWidgetUser->setItem(row, 1, new QTableWidgetItem(user.user.name));
        ui->tableWidgetUser->setItem(row, 2, new QTableWidgetItem(user.user.password));
        ui->tableWidgetUser->setItem(row, 3, new QTableWidgetItem(user.user.isSeller ? "是" : "否"));
        ui->tableWidgetUser->setItem(row, 4, new QTableWidgetItem(user.registerTime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidgetUser->setItem(row, 5, new QTableWidgetItem(user.lastLoginTime.toString("yyyy-MM-dd hh:mm:ss")));
    }
}

void AdminMainWindow::refreshSeller() {
    ui->tableWidgetSeller->clearContents();
    ui->tableWidgetSeller->setRowCount(0);
    auto sellers = DataInterface::AdminGetAllSellers();
    for (const auto &seller : sellers) {
        int row = ui->tableWidgetSeller->rowCount();
        ui->tableWidgetSeller->insertRow(row);
        ui->tableWidgetSeller->setItem(row, 0, new QTableWidgetItem(QString::number(seller.id)));
        ui->tableWidgetSeller->setItem(row, 1, new QTableWidgetItem(seller.phone));
        ui->tableWidgetSeller->setItem(row, 2, new QTableWidgetItem(seller.realName));
        ui->tableWidgetSeller->setItem(row, 3, new QTableWidgetItem(seller.realIdentityNumber));
    }
}

void AdminMainWindow::refreshShop() {
    ui->tableWidgetShop->clearContents();
    ui->tableWidgetShop->setRowCount(0);
    auto shops = DataInterface::AdminGetAllShops();
    for (const auto &shop : shops) {
        int row = ui->tableWidgetShop->rowCount();
        ui->tableWidgetShop->insertRow(row);
        ui->tableWidgetShop->setItem(row, 0, new QTableWidgetItem(QString::number(shop.id)));
        ui->tableWidgetShop->setItem(row, 1, new QTableWidgetItem(shop.name));
        ui->tableWidgetShop->setItem(row, 2, new QTableWidgetItem(shop.description));
        ui->tableWidgetShop->setItem(row, 3, new QTableWidgetItem(shop.status == 0 ? "正常" : "关闭"));
        ui->tableWidgetShop->setItem(row, 4, new QTableWidgetItem(shop.sellerName));
    }
}

void AdminMainWindow::refreshGoods() {
    ui->tableWidgetGoods->clearContents();
    ui->tableWidgetGoods->setRowCount(0);
    auto goods = DataInterface::AdminGetAllGoods();
    for (const auto &good : goods) {
        int row = ui->tableWidgetGoods->rowCount();
        ui->tableWidgetGoods->insertRow(row);
        ui->tableWidgetGoods->setItem(row, 0, new QTableWidgetItem(QString::number(good.id)));
        ui->tableWidgetGoods->setItem(row, 1, new QTableWidgetItem(good.name));
        ui->tableWidgetGoods->setItem(row, 2, new QTableWidgetItem(good.description));
        ui->tableWidgetGoods->setItem(row, 3, new QTableWidgetItem(QString::number(static_cast<double>(good.price))));
        ui->tableWidgetGoods->setItem(row, 4, new QTableWidgetItem(good.status == 0 ? "正常" : "下架"));
        ui->tableWidgetGoods->setItem(row, 5, new QTableWidgetItem(good.shopName));
    }
}

void AdminMainWindow::refreshApply() {
    ui->tableWidgetApply->clearContents();
    ui->tableWidgetApply->setRowCount(0);
    auto applies = DataInterface::AdminGetAllApplies();
    for (const auto &apply : applies) {
        int row = ui->tableWidgetApply->rowCount();
        ui->tableWidgetApply->insertRow(row);
        ui->tableWidgetApply->setItem(row, 0, new QTableWidgetItem(QString::number(apply.id)));
        ui->tableWidgetApply->setItem(row, 1, new QTableWidgetItem(QString::number(apply.userId)));
        ui->tableWidgetApply->setItem(row, 2, new QTableWidgetItem(apply.phone));
        ui->tableWidgetApply->setItem(row, 3, new QTableWidgetItem(apply.realName));
        ui->tableWidgetApply->setItem(row, 4, new QTableWidgetItem(apply.realIdentityNumber));
        ui->tableWidgetApply->setItem(row, 5, new QTableWidgetItem(apply.time.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidgetApply->setItem(row, 6, new QTableWidgetItem(apply.status == SellerApplyStatus::Pending ? "待审核" : (apply.status == SellerApplyStatus::Accept ? "通过" : "拒绝")));
    }
}

AdminMainWindow::~AdminMainWindow() {
    delete ui;
}