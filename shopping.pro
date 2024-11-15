QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shopping
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES -= QT_NO_DEBUG_OUTPUT

CONFIG+=c++2a

SOURCES += \
        main.cpp \

HEADERS += \

FORMS += \

RESOURCES += \
    resource.qrc

RC_FILE += logo.rc
