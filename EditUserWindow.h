#pragma once
#include <QMainWindow>

namespace Ui {
    class EditUserWindow;
}

class EditUserWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditUserWindow(QWidget *parent = nullptr);
    ~EditUserWindow();

private:
    Ui::EditUserWindow *ui;
};