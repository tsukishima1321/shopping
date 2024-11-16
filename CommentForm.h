#pragma once

#include <QWidget>
#include "DataInterface.h"

namespace Ui { class CommentForm; }

class CommentForm : public QWidget
{
    Q_OBJECT

public:
    CommentForm(QWidget *parent = nullptr);
    ~CommentForm();
    void setComment(const Comment &comment);

private:
    Ui::CommentForm *ui;
};