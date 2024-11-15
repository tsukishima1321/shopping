#include "DataInterface.h"
#include "DBInstance.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

User DataInterface::getUserById(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM users WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        User user;
        user.id = query.value("UserID").toUInt();
        user.name = query.value("UserName").toString();
        user.password = query.value("Password").toString();
        user.isSeller = query.value("isSeller").toBool();
        QSqlQuery query2(DBInstance::getInstance());
        query2.prepare("SELECT * FROM Address WHERE UserID = ?");
        query2.addBindValue(id);
        query2.exec();
        ID_t defaultAddressId = -1;
        user.defaultAddress = nullptr;
        while (query2.next()) {
            Address address;
            address.id = query2.value("AddressID").toUInt();
            address.addressText = query2.value("Address").toString();
            address.receiverName = query2.value("Receiver").toString();
            address.receiverPhone = query2.value("Phone").toString();
            if (query2.value("IsDefault").toBool()) {
                defaultAddressId = address.id;
            }
            user.addresses.append(address);
        }
        for(int i = 0; i < user.addresses.size(); i++){
            if(user.addresses[i].id == defaultAddressId){
                user.defaultAddress = &user.addresses[i];
                break;
            }
        }
        return user;
    }
    return User();
}

Seller DataInterface::getSellerById(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM SellerDetail WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        Seller seller;
        seller.id = query.value("UserID").toUInt();
        seller.name = query.value("UserName").toString();
        seller.password = query.value("Password").toString();
        seller.isSeller = true;
        seller.phone = query.value("Phone").toString();
        seller.realIdentityNumber = query.value("IDNumber").toString();
        seller.realName = query.value("RealName").toString();
        QSqlQuery query2(DBInstance::getInstance());
        query2.prepare("SELECT * FROM Address WHERE UserID = ?");
        query2.addBindValue(id);
        query2.exec();
        ID_t defaultAddressId = -1;
        seller.defaultAddress = nullptr;
        while (query2.next()) {
            Address address;
            address.id = query2.value("AddressID").toUInt();
            address.addressText = query2.value("Address").toString();
            address.receiverName = query2.value("Receiver").toString();
            address.receiverPhone = query2.value("Phone").toString();
            if (query2.value("IsDefault").toBool()) {
                defaultAddressId = address.id;
            }
            seller.addresses.append(address);
        }
        for (Address &address : seller.addresses) {
            if (address.id == defaultAddressId) {
                seller.defaultAddress = &address;
                break;
            }
        }
        return seller;
    }
    return Seller();
}

Shop DataInterface::getShopById(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM ShopDetail WHERE ShopID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        Shop shop;
        shop.id = query.value("ShopID").toUInt();
        shop.name = query.value("ShopName").toString();
        shop.description = query.value("Description").toString();
        shop.status = query.value("Status").toInt();
        shop.sellerId = query.value("UserID").toUInt();
        shop.sellerName = query.value("UserName").toString();
        return shop;
    }
    return Shop();
}

Goods DataInterface::getGoodsById(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM GoodsDetail WHERE GoodsID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        Goods goods;
        goods.id = query.value("GoodsID").toUInt();
        goods.name = query.value("GoodsName").toString();
        goods.description = query.value("Description").toString();
        goods.price = query.value("Price").toDouble();
        goods.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            goods.image = query.value("Image").toString();
        }
        goods.shopId = query.value("ShopID").toUInt();
        goods.shopName = query.value("ShopName").toString();
        return goods;
    }
    return Goods();
}

QVector<Goods> DataInterface::getGoodsByShopId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM GoodsDetail WHERE ShopID = ?");
    query.addBindValue(id);
    query.exec();
    QVector<Goods> goodsList;
    while (query.next()) {
        ID_t goodsId = query.value("GoodsID").toUInt();
        goodsList.append(getGoodsById(goodsId));
    }
    return goodsList;
}

QVector<Shop> DataInterface::getShopsBySellerId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM ShopDetail WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    QVector<Shop> shopList;
    while (query.next()) {
        ID_t shopId = query.value("ShopID").toUInt();
        shopList.append(getShopById(shopId));
    }
    return shopList;
}

UserPermission DataInterface::getUserPermissionByUserId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM UserPermission WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        UserPermission permission;
        permission.allowLogin = query.value("AllowLogin").toBool();
        permission.AllowShopping = query.value("AllowShopping").toBool();
        permission.allowComment = query.value("AllowComment").toBool();
        permission.allowAddShop = query.value("AllowAddShop").toBool();
        permission.allowAddGoods = query.value("AllowAddGoods").toBool();
        permission.allowHandleOrder = query.value("AllowHandleOrder").toBool();
        return permission;
    }
    return UserPermission();
}

QVector<GoodsWithQuantity> DataInterface::getGoodsInUserCart(ID_t UserId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM CartGoodsDetail WHERE UserID = ?");
    query.addBindValue(UserId);
    query.exec();
    QVector<GoodsWithQuantity> goodsList;
    while (query.next()) {
        GoodsWithQuantity goodsWithQuantity;
        ID_t goodsId = query.value("GoodsID").toUInt();
        goodsWithQuantity.goods.id = goodsId;
        goodsWithQuantity.goods.name = query.value("GoodsName").toString();
        goodsWithQuantity.goods.description = query.value("Description").toString();
        goodsWithQuantity.goods.price = query.value("Price").toDouble();
        goodsWithQuantity.goods.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            goodsWithQuantity.goods.image = query.value("Image").toString();
        }
        goodsWithQuantity.goods.shopId = query.value("ShopID").toUInt();
        goodsWithQuantity.goods.shopName = query.value("ShopName").toString();
        goodsWithQuantity.quantity = query.value("Quantity").toUInt();
        goodsWithQuantity.totalPrice = goodsWithQuantity.goods.price * goodsWithQuantity.quantity;
        goodsList.append(goodsWithQuantity);
    }
    return goodsList;
}

QVector<GoodsWithQuantity> DataInterface::getGoodsInOrder(ID_t OrderId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM OrderGoodsDetail WHERE OrderId = ?");
    query.addBindValue(OrderId);
    query.exec();
    QVector<GoodsWithQuantity> goodsList;
    while (query.next()) {
        GoodsWithQuantity goodsWithQuantity;
        ID_t goodsId = query.value("GoodsID").toUInt();
        goodsWithQuantity.goods.id = goodsId;
        goodsWithQuantity.goods.name = query.value("GoodsName").toString();
        goodsWithQuantity.goods.description = query.value("Description").toString();
        goodsWithQuantity.goods.price = query.value("Price").toDouble();
        goodsWithQuantity.goods.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            goodsWithQuantity.goods.image = query.value("Image").toString();
        }
        goodsWithQuantity.goods.shopId = query.value("ShopID").toUInt();
        goodsWithQuantity.goods.shopName = query.value("ShopName").toString();
        goodsWithQuantity.quantity = query.value("Quantity").toUInt();
        goodsWithQuantity.totalPrice = goodsWithQuantity.goods.price * goodsWithQuantity.quantity;
        goodsList.append(goodsWithQuantity);
    }
    return goodsList;
}

QVector<Goods> DataInterface::searchGoodsByName(const QString &name, GoodsOrder order, bool searchName, bool searchDescription) {
    QSqlQuery query(DBInstance::getInstance());
    QString sql = "SELECT * FROM GoodsDetail WHERE ";
    if (searchName) {
        sql += "GoodsName LIKE '%" + name + "%'";
        if (searchDescription) {
            sql += " OR Description LIKE '%" + name + "%'";
        }
    } else if (searchDescription) {
        sql += "Description LIKE '%" + name + "%'";
    }
    switch (order) {
    case GoodsOrder::GoodsPriceAscending:
        sql += " ORDER BY Price ASC";
        break;
    case GoodsOrder::GoodsPriceDescending:
        sql += " ORDER BY Price DESC";
        break;
    case GoodsOrder::GoodsNameAscending:
        sql += " ORDER BY GoodsName ASC";
        break;
    case GoodsOrder::GoodsNameDescending:
        sql += " ORDER BY GoodsName DESC";
        break;
    }
    query.prepare(sql);
    query.exec();
    QVector<Goods> goodsList;
    while (query.next()) {
        Goods goods;
        goods.id = query.value("GoodsID").toUInt();
        goods.name = query.value("GoodsName").toString();
        goods.description = query.value("Description").toString();
        goods.price = query.value("Price").toDouble();
        goods.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            goods.image = query.value("Image").toString();
        }
        goods.shopId = query.value("ShopID").toUInt();
        goods.shopName = query.value("ShopName").toString();
        goodsList.append(goods);
    }
    return goodsList;
}

QVector<Shop> DataInterface::searchShopsByName(const QString &name, ShopOrder order, bool searchName, bool searchDescription) {
    QSqlQuery query(DBInstance::getInstance());
    QString sql = "SELECT * FROM ShopDetail WHERE ";
    if (searchName) {
        sql += "ShopName LIKE '%" + name + "%'";
        if (searchDescription) {
            sql += " OR Description LIKE '%" + name + "%'";
        }
    } else if (searchDescription) {
        sql += "Description LIKE '%" + name + "%'";
    }
    switch (order) {
    case ShopOrder::ShopNameAscending:
        sql += " ORDER BY ShopName ASC";
        break;
    case ShopOrder::ShopNameDescending:
        sql += " ORDER BY ShopName DESC";
        break;
    }
    query.prepare(sql);
    query.exec();
    QVector<Shop> shopList;
    while (query.next()) {
        Shop shop;
        shop.id = query.value("ShopID").toUInt();
        shop.name = query.value("ShopName").toString();
        shop.description = query.value("Description").toString();
        shop.status = query.value("Status").toInt();
        shop.sellerId = query.value("UserID").toUInt();
        shop.sellerName = query.value("UserName").toString();
        shopList.append(shop);
    }
    return shopList;
}

std::optional<ID_t> DataInterface::UserLogin(const QString &name, const QString &password) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_UserLogin ?, ?");
    query.addBindValue(name);
    query.addBindValue(password);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return std::nullopt;
    }
    if (query.next()) {
        return query.value("UserID").toUInt();
    }
    return std::nullopt;
}

std::optional<ID_t> DataInterface::AdminLogin(const QString &name, const QString &password) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT AdminID FROM Admin WHERE AdminName = ? AND Password = ?");
    query.addBindValue(name);
    query.addBindValue(password);
    query.exec();
    if (query.next()) {
        return query.value("AdminID").toUInt();
    }
    return std::nullopt;
}

std::optional<ID_t> DataInterface::UserRegist(const QString &name, const QString &password) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_UserRegist ?, ?");
    query.addBindValue(name);
    query.addBindValue(password);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return std::nullopt;
    }
    if (query.next()) {
        return query.value("UserID").toUInt();
    }
    return std::nullopt;
}