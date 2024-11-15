#pragma once
#include <QDialog>
#include <QString>
#include <QWidget>

namespace Ui {
    class Login;
}

class Login : public QDialog {
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void pushButton_clicked();
    void lineEditName_returnPressed();
    void registButton_clicked();

private:
    Ui::Login *ui;
};
