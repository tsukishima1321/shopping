#pragma once

#include "Types.h"
#include <QMainWindow>

namespace Ui {
    class GoodsEditWindow;
}

class GoodsEditWindow : public QMainWindow {
    Q_OBJECT

signals:
    void goodsChanged();

public:
    GoodsEditWindow(QWidget *parent = nullptr);
    ~GoodsEditWindow();
    void setGoodsId(ID_t id);
    void setShopId(ID_t id);

private:
    Goods goods;
    Ui::GoodsEditWindow *ui;
    void loadGoodsInfo();
    void submit();
    void cancel();
    void uploadImage();
};