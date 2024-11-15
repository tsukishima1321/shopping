#pragma once
#include <QMainWindow>

namespace Ui {
    class UserRegistWindow;
}

class UserRegistWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit UserRegistWindow(QWidget *parent = nullptr);
    ~UserRegistWindow() override;

private:
    Ui::UserRegistWindow *ui;
    void registButtonClicked();
};