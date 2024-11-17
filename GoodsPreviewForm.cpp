#include "GoodsPreviewForm.h"
#include "ui_GoodsPreviewForm.h"
#include "Iconresources.h"
#include <QPainter>

GoodsPreviewForm::GoodsPreviewForm(QWidget *parent) :
        QWidget(parent), ui(new Ui::GoodsPreviewForm) {
    ui->setupUi(this);
    available = true;
    this->setMinimumHeight(100);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->hideElements();
    ui->labelDes->setWordWrap(true);
    ui->labelDes->setAlignment(Qt::AlignTop);
    ui->labelText->setWordWrap(true);
    ui->labelText->setAlignment(Qt::AlignTop);
    ui->labelImg->setPixmap(IconResources::getPixmaps()["default-goods"].scaled(ui->labelImg->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

GoodsPreviewForm::~GoodsPreviewForm() {
    delete ui;
}

void GoodsPreviewForm::setImage(const QPixmap &image) {
    QPixmap pixmap = image.scaled(ui->labelImg->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    ui->labelImg->setPixmap(pixmap);
}

void GoodsPreviewForm::hideElements() {
    ui->labelDes->hide();
    ui->labelText->hide();
    ui->labelPrice->hide();
    ui->labelImg->hide();
    ui->labelOmit->hide();
    ui->checkBox->hide();
    available = true;
}

void GoodsPreviewForm::setText(const QString &name, const QString &price, const QString &description) {
    ui->labelText->setText(name);
    ui->labelDes->setText(description);
    ui->labelPrice->setText("单价：" + price);
    ui->labelText->show();
    ui->labelDes->show();
    ui->labelPrice->show();
    ui->labelImg->show();
    this->adjustSize();
    available = false;
}

bool GoodsPreviewForm::isAvailable() const {
    return available;
}

ID_t GoodsPreviewForm::getId() const {
    return id;
}

void GoodsPreviewForm::setId(ID_t id) {
    this->id = id;
}

int GoodsPreviewForm::getHeight() const {
    return this->height();
}

void GoodsPreviewForm::setOmit(bool omit) {
    if (omit) {
        ui->labelOmit->show();
    } else {
        ui->labelOmit->hide();
    }
}

void GoodsPreviewForm::mouseDoubleClickEvent(QMouseEvent *event) {
    (void)event;
    emit isClicked(id);
}

void GoodsPreviewForm::paintEvent(QPaintEvent *event) {
    (void)event;
    if (available) {
        return QWidget::paintEvent(event);
    }
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(0, 0, this->width(), this->height());
    return QWidget::paintEvent(event);
}

void GoodsPreviewForm::enableCheck(bool enabled) {
    ui->checkBox->setHidden(!enabled);
    ui->checkBox->setEnabled(enabled);
}

bool GoodsPreviewForm::isChecked() const {
    return ui->checkBox->isChecked();
}

void GoodsPreviewForm::setChecked(bool checked) {
    ui->checkBox->setChecked(checked);
}