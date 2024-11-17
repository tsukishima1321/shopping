#pragma once
#include "Types.h"
#include <QWidget>

namespace Ui {
    class ShopPreviewForm;
}

class ShopPreviewForm : public QWidget {
    Q_OBJECT
public:
    explicit ShopPreviewForm(QWidget *parent = nullptr);
    ~ShopPreviewForm();
    void setShop(Shop shop);

private:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    Shop shop;
    Ui::ShopPreviewForm *ui;
};
