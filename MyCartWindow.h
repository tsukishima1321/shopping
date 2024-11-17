#pragma once

#include <QMainWindow>
#include <QLayout>
#include "GoodsWithQuantityForm.h"

namespace Ui { class MyCartWindow; }

class MyCartWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyCartWindow(QWidget *parent = nullptr);
    ~MyCartWindow();

private:
    Ui::MyCartWindow *ui;
    QVector<GoodsWithQuantity> goodsList;
    QVBoxLayout *vBoxLayout;

    void updateGoods();
    void updateTotalPrice();
};