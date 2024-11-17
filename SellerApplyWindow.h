#pragma once

#include <QMainWindow>

namespace Ui {
    class SellerApplyWindow;
}

class SellerApplyWindow : public QMainWindow {
    Q_OBJECT

public:
    SellerApplyWindow(QWidget *parent = nullptr);
    ~SellerApplyWindow();

private:
    Ui::SellerApplyWindow *ui;
    void submitApply();
};