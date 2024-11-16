#pragma once
#include <QMainWindow>
#include <QPushButton>
#include "DataInterface.h"

namespace Ui {
    class EditUserWindow;
}

class DeleteButton : public QPushButton {
    Q_OBJECT
public:
    explicit DeleteButton(ID_t addressID, QWidget *parent = nullptr);
    ID_t addressId;
    ~DeleteButton();
signals:
    void addressDeleted();
};

class SetDefaultButton : public QPushButton {
    Q_OBJECT
public:
    explicit SetDefaultButton(ID_t addressID, QWidget *parent = nullptr);
    ID_t addressId;
    ~SetDefaultButton();
signals:
    void defaultAddressChanged();
};

class EditUserWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditUserWindow(QWidget *parent = nullptr);
    ~EditUserWindow();

signals:
    void userChanged();

private:
    void newAddress();
    void refreshAddressTable();
    void submitChanges();
    void cancelChanges();
    void sellerApply();
    User user;
    QVector<Address> addresses;
    Ui::EditUserWindow *ui;
};