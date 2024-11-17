#pragma once

#include <QWidget>
#include "DataInterface.h"

namespace Ui { class GoodsWithQuantityForm; }

class GoodsWithQuantityForm : public QWidget
{
    Q_OBJECT

signals:
    void isClicked(ID_t id);
    void quantityChanged(ID_t id, int quantity);
    void isDeleted(ID_t id);

public:
    void setGoods(const Goods &goods, int quantity);
    ID_t getId() const;
    bool isActive();
    int getQuantity();
    GoodsWithQuantityForm(QWidget *parent = nullptr);
    ~GoodsWithQuantityForm();

private:
    Ui::GoodsWithQuantityForm *ui;
    ID_t id;
    Price price;
    bool active;
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

};