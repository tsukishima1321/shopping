#include "GoodsEditWindow.h"
#include "ui_GoodsEditWindow.h"
#include "DataInterface.h"

GoodsEditWindow::GoodsEditWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GoodsEditWindow)
{
    ui->setupUi(this);
}

void GoodsEditWindow::setGoodsId(ID_t id)
{
    (void)id;
}

GoodsEditWindow::~GoodsEditWindow()
{
    delete ui;
}