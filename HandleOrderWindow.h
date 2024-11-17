#pragma once

#include <QMainWindow>

namespace Ui { class HandleOrderWindow; }

class HandleOrderWindow : public QMainWindow
{
    Q_OBJECT

public:
    HandleOrderWindow(QWidget *parent = nullptr);
    ~HandleOrderWindow();

private:
    Ui::HandleOrderWindow *ui;
    void loadOrders();
};