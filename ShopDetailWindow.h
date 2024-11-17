#pragma once

#include "GoodsPreviewForm.h"
#include "Types.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class ShopDetailWindow;
}

class ShopDetailWindow : public QMainWindow {
    Q_OBJECT

public:
    ShopDetailWindow(ID_t shopID, QWidget *parent = nullptr);
    ~ShopDetailWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ShopDetailWindow *ui;
    Shop shop;

    QVector<Goods> goodsList;

    int currentColumnCount;

    QHBoxLayout *hBoxLayout;
    QList<QVBoxLayout *> vBoxLayouts;
    QVector<GoodsPreviewForm *> previewList;

    void locateGoods();
    GoodsPreviewForm *addGoodsItem(QString image, QString name, QString price, QString des, ID_t id);

    void openDetailMenu(ID_t id);
};