#pragma once
#include "ShopPreviewForm.h"
#include <QLayout>
#include <QMainWindow>

namespace Ui {
    class UserMainWindow;
}

class UserMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit UserMainWindow(QWidget *parent = nullptr);
    ~UserMainWindow();

private:
    void buttonSearchClicked();
    void buttonStartShoppingClicked();
    void buttonAddShopClicked();
    void buttonEditUserClicked();
    void buttonMyCollectionClicked();
    void buttonMyOrderClicked();
    void buttonMyCartClicked();
    void buttonRefreshClicked();
    void refreshUser();

    Ui::UserMainWindow *ui;

    QVBoxLayout *vBoxLayout;
};