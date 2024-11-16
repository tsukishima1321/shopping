#pragma once

#include <QMainWindow>
#include <QLayout>
#include "DataInterface.h"

namespace Ui { class GoodsDetailWindow; }

class GoodsDetailWindow : public QMainWindow
{
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