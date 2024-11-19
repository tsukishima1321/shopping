#include "SearchWindow.h"
#include "DataInterface.h"
#include "GlobalConfig.h"
#include "GoodsDetailWindow.h"
#include "ui_SearchWindow.h"
#include <QMessageBox>

SearchWindow::SearchWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::SearchWindow),
        currentColumnCount(3),
        currentPage(1) {
    ui->setupUi(this);
    ui->radioButtonDesc->setChecked(true);
    ui->checkBoxName->setChecked(true);
    ui->dateEditFrom->setDate(QDate::currentDate());
    ui->dateEditTo->setDate(QDate::currentDate());

    hBoxLayout = new QHBoxLayout;
    for (int i = 0; i < currentColumnCount; i++) {
        QVBoxLayout *column = new QVBoxLayout;
        column->setAlignment(Qt::AlignTop);
        hBoxLayout->addLayout(column);
        vBoxLayouts.append(column);
    }
    ui->scrollAreaWidgetContents->setLayout(hBoxLayout);

    currentPage = ui->pageNavigate->getCurrentPage();
    previewList = new GoodsPreviewForm[pageSize];
    previewListSpan = std::span<GoodsPreviewForm>(previewList, pageSize);
    for (auto &&form : previewListSpan) {
        connect(&form, &GoodsPreviewForm::isClicked, this, &SearchWindow::openDetailMenu);
    }

    connect(ui->pageNavigate, &PageNavigator::currentPageChanged, this, [this](int p) {
        if (p != currentPage) {
            currentPage = p;
            updateSearch();
        }
        currentPage = p;
    });

    connect(ui->radioButtonAsc, &QRadioButton::clicked, this, [this]() {
        buttonSearchClicked();
    });
    connect(ui->radioButtonDesc, &QRadioButton::clicked, this, [this]() {
        buttonSearchClicked();
    });

    connect(ui->checkBoxDate, &QCheckBox::stateChanged, this, [this](int stat) {
        if (stat == Qt::CheckState::Checked) {
            ui->dateEditFrom->setEnabled(true);
            ui->dateEditTo->setEnabled(true);
        } else {
            ui->dateEditFrom->setEnabled(false);
            ui->dateEditTo->setEnabled(false);
        }
    });

    connect(ui->comboBoxOrder, &QComboBox::currentIndexChanged, this, [this](int i) {
        (void)i;
        buttonSearchClicked();
    });

    connect(ui->searchButton, &QPushButton::clicked, this, &SearchWindow::buttonSearchClicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &SearchWindow::buttonSearchClicked);
}

SearchWindow::SearchWindow(const QString &searchText, QWidget *parent) :
        SearchWindow(parent) {
    ui->lineEdit->setText(searchText);
    buttonSearchClicked();
}

void SearchWindow::buttonSearchClicked() {
    GoodsOrder order;
    switch (ui->comboBoxOrder->currentIndex()) {
    case 0:
        if (ui->radioButtonAsc->isChecked()) {
            order = GoodsOrder::GoodsNameAscending;
        } else {
            order = GoodsOrder::GoodsNameDescending;
        }
        break;
    case 1:
        if (ui->radioButtonAsc->isChecked()) {
            order = GoodsOrder::GoodsPriceAscending;
        } else {
            order = GoodsOrder::GoodsPriceDescending;
        }
        break;
    default:
        order = GoodsOrder::GoodsNameAscending;
    }
    bool searchName = ui->checkBoxName->isChecked();
    bool searchDescription = ui->checkBoxDes->isChecked();
    goodsList = DataInterface::searchGoodsByName(ui->lineEdit->text(), order, searchName, searchDescription);
    ui->pageNavigate->setMaxPage(goodsList.length() / pageSize + 1);
    ui->pageNavigate->setCurrentPage(1, true);
    currentPage = 1;
    updateSearch();
}

void SearchWindow::updateSearch() {
    for (auto &&form : previewListSpan) {
        form.~GoodsPreviewForm();
        new (&form) GoodsPreviewForm;
        connect(&form, &GoodsPreviewForm::isClicked, this, &SearchWindow::openDetailMenu);
    }
    for (int i = (currentPage - 1) * pageSize; i < (currentPage)*pageSize && i < goodsList.length(); i++) {
        const Goods &goods = goodsList[i];
        addGoodsItem(goods.image, goods.name, goods.price.toString(), goods.description, goods.id);
    }
    for (QVBoxLayout *column : vBoxLayouts) {
        for (int i = column->count() - 1; i >= 0; i--) {
            column->removeItem(column->itemAt(i));
        }
    }
    locateGoods();
}

GoodsPreviewForm *SearchWindow::addGoodsItem(QString image, QString name, QString price, QString des, ID_t id) {
    GoodsPreviewForm *formToAdd = nullptr;
    for (auto &&form : previewListSpan) {
        if (form.isAvailable()) {
            formToAdd = &form;
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

void SearchWindow::locateGoods() {
    for (auto &&form : previewListSpan) {
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
            columnMinHeight->addWidget(&form);
        }
    }
}

void SearchWindow::resizeEvent(QResizeEvent *event) {
    (void)event;
    int width = ui->scrollArea->width();
    if (ui->scrollArea->width() == 640) {
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

SearchWindow::~SearchWindow() {
    delete[] previewList;
    if (hBoxLayout) {
        delete hBoxLayout;
    }
    delete ui;
}

void SearchWindow::openDetailMenu(ID_t id) {
    GoodsDetailWindow *detailWindow = new GoodsDetailWindow(id);
    detailWindow->show();
}