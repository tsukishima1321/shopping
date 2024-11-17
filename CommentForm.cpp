#include "CommentForm.h"
#include "ui_CommentForm.h"

CommentForm::CommentForm(QWidget *parent) :
        QWidget(parent), ui(new Ui::CommentForm) {
    ui->setupUi(this);
}

CommentForm::~CommentForm() {
    delete ui;
}

void CommentForm::setComment(const Comment &comment) {
    ui->labelUserName->setText(comment.userName);
    ui->labelCommentTime->setText(comment.time.toString());
    ui->labelContent->setText(comment.content);
}