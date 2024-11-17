#pragma once

#include "Types.h"
#include <QWidget>

namespace Ui {
    class GoodsPreviewForm;
}

class GoodsPreviewForm : public QWidget {
    Q_OBJECT
signals:
    void isClicked(ID_t id);

public:
    GoodsPreviewForm(QWidget *parent = nullptr);
    ~GoodsPreviewForm();
    void setImage(const QPixmap &image);
    void setText(const QString &name, const QString &price, const QString &description);
    void hideElements();
    bool isAvailable() const;
    ID_t getId() const;
    void setId(ID_t id);
    int getHeight() const;
    void setOmit(bool omit);

    void enableCheck(bool enabled);
    bool isChecked() const;
    void setChecked(bool checked);

private:
    Ui::GoodsPreviewForm *ui;
    bool available;
    ID_t id;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};