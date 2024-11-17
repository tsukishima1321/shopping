#pragma once

#include "GoodsPreviewForm.h"
#include "Types.h"
#include <QLayout>
#include <QMainWindow>
#include <span>

namespace Ui {
    class SearchWindow;
}

constexpr int pageSize = 20;

class SearchWindow : public QMainWindow {
    Q_OBJECT

public:
    SearchWindow(QWidget *parent = nullptr);
    SearchWindow(const QString &searchText, QWidget *parent = nullptr);
    ~SearchWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::SearchWindow *ui;

    QVector<Goods> goodsList;

    int currentColumnCount;
    QHBoxLayout *hBoxLayout;
    QList<QVBoxLayout *> vBoxLayouts;
    GoodsPreviewForm *previewList;
    std::span<GoodsPreviewForm> previewListSpan;

    void buttonSearchClicked();
    void updateSearch();
    void locateGoods();
    GoodsPreviewForm *addGoodsItem(QString image, QString name, QString price, QString des, ID_t id);

    void openDetailMenu(ID_t id);

    int currentPage;
};