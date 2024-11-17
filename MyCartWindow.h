#pragma once

#include "GoodsWithQuantityForm.h"
#include "Types.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class MyCartWindow;
}

class MyCartWindow : public QMainWindow {
    Q_OBJECT

public:
    MyCartWindow(QWidget *parent = nullptr);
    ~MyCartWindow();

private:
    Ui::MyCartWindow *ui;
    QVector<GoodsWithQuantity> goodsList;
    QVBoxLayout *vBoxLayout;

    QList<Address> addresses;

    void updateGoods();
    void updateTotalPrice();

    void subMitOrder();
};