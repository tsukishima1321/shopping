#pragma once

#include "Types.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class OrderDetailView;
}

class OrderDetailView : public QMainWindow {
    Q_OBJECT

public:
    OrderDetailView(const Order &order, QWidget *parent = nullptr);
    ~OrderDetailView();

private:
    Ui::OrderDetailView *ui;
    const Order &order;

    QVBoxLayout *vBoxLayout;
};