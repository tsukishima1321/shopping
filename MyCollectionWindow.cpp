#include "MyCollectionWindow.h"
#include "CurrentUser.h"
#include "GlobalConfig.h"
#include "GoodsDetailWindow.h"
#include "ui_MyCollectionWindow.h"
#include <QMessageBox>

MyCollectionWindow::MyCollectionWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MyCollectionWindow),
        currentColumnCount(3) {
    ui->setupUi(this);

    hBoxLayout = new QHBoxLayout;
    for (int i = 0; i < currentColumnCount; i++) {
        QVBoxLayout *column = new QVBoxLayout;
        column->setAlignment(Qt::AlignTop);
        hBoxLayout->addLayout(column);
        vBoxLayouts.append(column);
    }
    ui->scrollAreaWidgetContents->setLayout(hBoxLayout);

    updateCollection();

    connect(ui->checkBoxSelectAll, &QCheckBox::stateChanged, [this](int state) {
        for (auto &&form : previewList) {
            form->setChecked(state == Qt::Checked);
        }
    });

    connect(ui->buttonDelete, &QPushButton::clicked, [this]() {
        for (auto &&form : previewList) {
            if (form->isChecked()) {
                if (!DataInterface::RemoveGoodsFromCollect(CurrentUser::getInstance()->getUserId(), form->getId())) {
                    QMessageBox::warning(this, "错误", "删除收藏失败");
                }
            }
        }
        updateCollection();
    });
}

void MyCollectionWindow::updateCollection() {
    goodsList = DataInterface::getCollectionGoodsByUserId(CurrentUser::getInstance()->getUserId());

    for (auto &&form : previewList) {
        delete form;
    }
    previewList.clear();

    for (const Goods &goods : goodsList) {
        previewList.append(new GoodsPreviewForm);
        addGoodsItem(goods.image, goods.name, goods.price.toString(), goods.description, goods.id);
    }
    for (auto &&form : previewList) {
        form->enableCheck(true);
        connect(form, &GoodsPreviewForm::isClicked, this, &MyCollectionWindow::openDetailMenu);
    }
    for (QVBoxLayout *column : vBoxLayouts) {
        for (int i = column->count() - 1; i >= 0; i--) {
            column->removeItem(column->itemAt(i));
        }
    }
    locateGoods();
}

GoodsPreviewForm *MyCollectionWindow::addGoodsItem(QString image, QString name, QString price, QString des, ID_t id) {
    GoodsPreviewForm *formToAdd = nullptr;
    for (auto &&form : previewList) {
        if (form->isAvailable()) {
            formToAdd = form;
            break;
        }
    }
    if (formToAdd) {
        formToAdd->setId(id);
        if (!image.isEmpty()) {
            QString imagePath = GlobalConfig::getInstance()->getStaticPath() + image;
            formToAdd->setImage(QPixmap(imagePath));
        }
        formToAdd->setText(name, price, des);
    }
    return formToAdd;
}

void MyCollectionWindow::locateGoods() {
    for (auto &&form : previewList) {
        QVBoxLayout *columnMinHeight = nullptr;
        int minHeight = 999999;
        for (QVBoxLayout *column : vBoxLayouts) {
            int height = 0;
            for (int i = column->count() - 1; i >= 0; i--) {
                height += dynamic_cast<GoodsPreviewForm *>(column->itemAt(i)->widget())->getHeight();
            }
            if (height < minHeight) {
                // qDebug() << height;
                columnMinHeight = column;
                minHeight = height;
            }
        }
        if (columnMinHeight) {
            columnMinHeight->addWidget(form);
        }
    }
}

void MyCollectionWindow::resizeEvent(QResizeEvent *event) {
    (void)event;
    int width = ui->scrollArea->width();
    if (ui->scrollArea->width() == 100) {
        currentColumnCount = 3;
    } else {
        currentColumnCount = width / 300;
    }
    for (QVBoxLayout *column : vBoxLayouts) {
        for (int i = column->count() - 1; i >= 0; i--) {
            column->removeItem(column->itemAt(i));
        }
    }
    QLayoutItem *column;
    while ((column = hBoxLayout->itemAt(0))) {
        hBoxLayout->removeItem(column);
        delete column;
    }
    vBoxLayouts.clear();
    for (int i = 0; i < currentColumnCount; i++) {
        QVBoxLayout *column = new QVBoxLayout();
        column->setAlignment(Qt::AlignTop);
        hBoxLayout->addLayout(column);
        vBoxLayouts.append(column);
    }
    locateGoods();
    QMainWindow::resizeEvent(event);
}

void MyCollectionWindow::openDetailMenu(ID_t id) {
    GoodsDetailWindow *detailWindow = new GoodsDetailWindow(id);
    detailWindow->show();
}

MyCollectionWindow::~MyCollectionWindow() {
    if (hBoxLayout) {
        delete hBoxLayout;
    }
    delete ui;
}