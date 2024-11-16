#pragma once

#include <QWidget>
#include "DataInterface.h"

namespace Ui { class EditUserPermission; }

class EditUserPermission : public QWidget
{
    Q_OBJECT

public:
    EditUserPermission(ID_t userID, QWidget *parent = nullptr);
    ~EditUserPermission();

private:
    ID_t userId;
    Ui::EditUserPermission *ui;
    void submit();
    void cancel();
    void blockAll();
    void refresh();
};