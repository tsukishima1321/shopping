#ifndef ICONRESOURCES_H
#define ICONRESOURCES_H

#include <QIcon>
#include <QMap>
#include <QPixmap>

class IconResources {
public:
    static const QMap<QString, QIcon> &getIcons();
    QMap<QString, QIcon> icons;
    static const QMap<QString, QPixmap> &getPixmaps();
    QMap<QString, QPixmap> pixmaps;

protected:
    IconResources();
    static IconResources *_instance;
};

#endif // ICONRESOURCES_H
