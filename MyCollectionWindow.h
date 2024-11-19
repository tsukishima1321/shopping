#pragma once

#include "GoodsPreviewForm.h"
#include "Types.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class MyCollectionWindow;
}

class MyCollectionWindow : public QMainWindow {
    Q_OBJECT

public:
    MyCollectionWindow(QWidget *parent = nullptr);
    ~MyCollectionWindow();
    void updateCollection();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MyCollectionWindow *ui;

    QVector<Goods> goodsList;

    int currentColumnCount;
    QHBoxLayout *hBoxLayout;
    QList<QVBoxLayout *> vBoxLayouts;
    QVector<GoodsPreviewForm *> previewList;

    void locateGoods();
    GoodsPreviewForm *addGoodsItem(QString image, QString name, QString price, QString des, ID_t id);

    void openDetailMenu(ID_t id);
};