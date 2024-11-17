#pragma once

#include "Types.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class GoodsDetailWindow;
}

class GoodsDetailWindow : public QMainWindow {
    Q_OBJECT

public:
    GoodsDetailWindow(ID_t goodsId, QWidget *parent = nullptr);
    ~GoodsDetailWindow();

private:
    Ui::GoodsDetailWindow *ui;
    Goods goods;

    QVBoxLayout *commentsLayout;

    void addComment();
    void refreshComments();

    void addGoodsToCart();
    void addGoodsToCollect();

    void openShop();
};