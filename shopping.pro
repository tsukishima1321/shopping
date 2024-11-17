QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shopping
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES -= QT_NO_DEBUG_OUTPUT

CONFIG+=c++2a

include     ($$PWD/pagenavigator/pagenavigator.pri)
include     ($$PWD/xcombobox/xcombobox.pri)

SOURCES += \
        main.cpp \
        DBInstance.cpp \
        GlobalConfig.cpp \
        DataInterface.cpp \
        CurrentUser.cpp \
        IconResources.cpp \
        login.cpp \
        UserMainWindow.cpp \
        ShopPreviewForm.cpp \
        UserRegistWindow.cpp \
        EditUserWindow.cpp \
        SellerApplyWindow.cpp \
        AdminMainWindow.cpp \
        EditUserPermission.cpp \
        SearchWindow.cpp \
        GoodsPreviewForm.cpp \
        GoodsDetailWindow.cpp \
        CommentForm.cpp \ 
        MyCollectionWindow.cpp \
        MyCartWindow.cpp \
        GoodsWithQuantityForm.cpp \
        MyOrderWindow.cpp \
        OrderDetailView.cpp \

HEADERS += \
        Types.h \
        DBInstance.h \
        GlobalConfig.h \
        DataInterface.h \
        CurrentUser.h \
        IconResources.h \
        login.h \
        UserMainWindow.h \
        ShopPreviewForm.h \
        UserRegistWindow.h \
        EditUserWindow.h \
        SellerApplyWindow.h \
        AdminMainWindow.h \
        EditUserPermission.h \
        SearchWindow.h \
        GoodsPreviewForm.h \
        GoodsDetailWindow.h \
        CommentForm.h \ 
        MyCollectionWindow.h \
        MyCartWindow.h \
        GoodsWithQuantityForm.h \
        MyOrderWindow.h \
        OrderDetailView.h \ 

FORMS += \
        login.ui \
        UserMainWindow.ui \
        ShopPreviewForm.ui \
        UserRegistWindow.ui \
        EditUserWindow.ui \
        SellerApplyWindow.ui \
        AdminMainWindow.ui \
        EditUserPermission.ui \
        SearchWindow.ui \
        GoodsPreviewForm.ui \
        GoodsDetailWindow.ui \
        CommentForm.ui \ 
        MyCollectionWindow.ui \
        MyCartWindow.ui \
        GoodsWithQuantityForm.ui \
        MyOrderWindow.ui \
        OrderDetailView.ui \ 

RESOURCES += \
    resource.qrc

RC_FILE += logo.rc
