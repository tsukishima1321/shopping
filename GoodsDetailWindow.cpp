#include "GoodsDetailWindow.h"
#include "CurrentUser.h"
#include "GlobalConfig.h"
#include "Iconresources.h"
#include "CommentForm.h"
#include "ui_GoodsDetailWindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

GoodsDetailWindow::GoodsDetailWindow(ID_t goodsId, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::GoodsDetailWindow) {
    ui->setupUi(this);
    goods = DataInterface::getGoodsById(goodsId);
    ui->labelShopIcon->setPixmap(IconResources::getPixmaps()["store"]);
    ui->labelGoodsDes->setText(goods.description);
    ui->labelGoodsName->setText(goods.name);
    ui->labelShopName->setText(goods.shopName);
    if (goods.image.isEmpty()) {
        ui->labelGoodsImage->setPixmap(IconResources::getPixmaps()["default-goods"].scaled(ui->labelGoodsImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        QString imagePath = GlobalConfig::getInstance()->getStaticPath() + goods.image;
        ui->labelGoodsImage->setPixmap(QPixmap(imagePath).scaled(ui->labelGoodsImage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    connect(ui->buttonCart, &QPushButton::clicked, this, &GoodsDetailWindow::addGoodsToCart);
    connect(ui->buttonCollect, &QPushButton::clicked, this, &GoodsDetailWindow::addGoodsToCollect);
    connect(ui->buttonAddComment, &QPushButton::clicked, this, &GoodsDetailWindow::addComment);
    connect(ui->buttonEnterShop, &QPushButton::clicked, this, &GoodsDetailWindow::openShop);

    commentsLayout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(commentsLayout);
    refreshComments();
}

void GoodsDetailWindow::refreshComments() {
    QVector<Comment> comments = DataInterface::getCommentsByGoodsId(goods.id);
    for (const Comment &comment : comments) {
        CommentForm *label = new CommentForm();
        label->setComment(comment);
        commentsLayout->addWidget(label);
    }
}

void GoodsDetailWindow::addComment() {
    UserPermission permission = DataInterface::getUserPermissionByUserId(CurrentUser::getInstance()->getUserId());
    if (!permission.allowComment) {
        QMessageBox::warning(this, "错误", "你已被禁止发布评论");
        return;
    }
    QDialog dialog;
    QVBoxLayout layout(&dialog);
    QLabel label("请输入评论");
    QLineEdit lineEdit;
    layout.addWidget(&label);
    layout.addWidget(&lineEdit);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout.addWidget(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        DataInterface::AddComment(CurrentUser::getInstance()->getUserId(), goods.id, lineEdit.text());
        refreshComments();
    }
}

void GoodsDetailWindow::addGoodsToCart() {
    // ask for quantity
    QDialog dialog;
    QVBoxLayout layout(&dialog);
    QLabel label("请输入数量");
    QLineEdit lineEdit;
    layout.addWidget(&label);
    layout.addWidget(&lineEdit);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout.addWidget(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        bool ok;
        int quantity = lineEdit.text().toInt(&ok);
        if (ok) {
            bool r = DataInterface::AddGoodsToCart(CurrentUser::getInstance()->getUserId(), goods.id, quantity);
            if (!r) {
                QMessageBox::warning(this, "错误", "添加失败");
            }else{
                QMessageBox::information(this, "成功", "添加成功");
            }
        }
    }
}

void GoodsDetailWindow::addGoodsToCollect() {
    bool r = DataInterface::AddGoodsToCollect(CurrentUser::getInstance()->getUserId(), goods.id);
    if (!r) {
        QMessageBox::warning(this, "错误", "你已经收藏过本商品");
    }else{
        QMessageBox::information(this, "成功", "收藏成功");
    }
}

void GoodsDetailWindow::openShop() {
    /*
    ShopDetailWindow *shopDetailWindow = new ShopDetailWindow(goods.shopId);
    shopDetailWindow->show();
    */
}

GoodsDetailWindow::~GoodsDetailWindow() {
    delete commentsLayout;
    delete ui;
}