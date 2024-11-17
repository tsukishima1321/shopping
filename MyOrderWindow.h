#pragma once

#include "Types.h"
#include <QMainWindow>

namespace Ui {
    class MyOrderWindow;
}

class MyOrderWindow : public QMainWindow {
    Q_OBJECT

public:
    MyOrderWindow(QWidget *parent = nullptr);
    ~MyOrderWindow();

private:
    Ui::MyOrderWindow *ui;
    QVector<Order> orders;
    void updateOrderView();
};