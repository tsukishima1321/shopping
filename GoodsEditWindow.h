#pragma once

#include <QMainWindow>
#include "Types.h"

namespace Ui { class GoodsEditWindow; }

class GoodsEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    GoodsEditWindow(QWidget *parent = nullptr);
    ~GoodsEditWindow();
    void setGoodsId(ID_t id);

private:
    Ui::GoodsEditWindow *ui;
};