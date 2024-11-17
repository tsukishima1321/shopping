#pragma once

#include <QMainWindow>
#include <QLayout>
#include "GoodsPreviewForm.h"
#include "Types.h"

namespace Ui { class ShopEditWindow; }

class ShopEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    ShopEditWindow(QWidget *parent = nullptr);
    ~ShopEditWindow();
    void setShopId(ID_t shopId);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ShopEditWindow *ui;
    Shop shop;

    void submit();
    void cancel();

    QVector<Goods> goodsList;

    int currentColumnCount;

    QHBoxLayout *hBoxLayout;
    QList<QVBoxLayout *> vBoxLayouts;
    QVector<GoodsPreviewForm *> previewList;

    void updateGoods();

    void locateGoods();
    GoodsPreviewForm *addGoodsItem(QString image, QString name, QString price, QString des, ID_t id);

    void openDetailMenu(ID_t id);
    void newGoods();
};