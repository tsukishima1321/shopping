#pragma once

#include <QMainWindow>
#include <DataInterface.h>

namespace Ui { class AdminMainWindow; }

class AdminMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminMainWindow(QWidget *parent = nullptr);
    ~AdminMainWindow();

private:
    Ui::AdminMainWindow *ui;
    void refreshUser();
    void refreshSeller();
    void refreshShop();
    void refreshGoods();
    void refreshApply();

    void closeShop(ID_t shopId);
    void openShop(ID_t shopId);
    void deActiveGoods(ID_t goodsId);
    void activeGoods(ID_t goodsId);
    void acceptApply(ID_t applyId);
    void rejectApply(ID_t applyId);
};