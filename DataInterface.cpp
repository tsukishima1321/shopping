#include "DataInterface.h"
#include "CurrentUser.h"
#include "DBInstance.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

User DataInterface::getUserById(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM Users WHERE UserID = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return User();
    }
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
        for (int i = 0; i < user.addresses.size(); i++) {
            if (user.addresses[i].id == defaultAddressId) {
                user.defaultAddress = &user.addresses[i];
                break;
            }
        }
        return user;
    }
    return User();
}

QVector<UserWithTime> DataInterface::AdminGetAllUsers() {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM Users");
    query.exec();
    QVector<UserWithTime> users;
    while (query.next()) {
        User user;
        user.id = query.value("UserID").toUInt();
        user.name = query.value("UserName").toString();
        user.password = query.value("Password").toString();
        user.isSeller = query.value("IsSeller").toBool();
        UserWithTime userWithTime;
        userWithTime.user = user;
        userWithTime.registerTime = query.value("RegisterTime").toDateTime();
        userWithTime.lastLoginTime = query.value("LastLoginTime").toDateTime();
        users.append(userWithTime);
    }
    return users;
}

QVector<Seller> DataInterface::AdminGetAllSellers() {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM SellerDetail");
    query.exec();
    QVector<Seller> sellers;
    while (query.next()) {
        Seller seller;
        seller.id = query.value("UserID").toUInt();
        seller.name = query.value("UserName").toString();
        seller.isSeller = true;
        seller.phone = query.value("Phone").toString();
        seller.realIdentityNumber = query.value("IDNumber").toString();
        seller.realName = query.value("RealName").toString();
        sellers.append(seller);
    }
    return sellers;
}

QVector<Shop> DataInterface::AdminGetAllShops() {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM ShopDetail");
    query.exec();
    QVector<Shop> shops;
    while (query.next()) {
        Shop shop;
        shop.id = query.value("ShopID").toUInt();
        shop.name = query.value("ShopName").toString();
        shop.description = query.value("Description").toString();
        shop.status = query.value("Status").toInt();
        shop.sellerId = query.value("UserID").toUInt();
        shop.sellerName = query.value("UserName").toString();
        shops.append(shop);
    }
    return shops;
}

QVector<Goods> DataInterface::AdminGetAllGoods() {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM GoodsDetail");
    query.exec();
    QVector<Goods> goods;
    while (query.next()) {
        Goods good;
        good.id = query.value("GoodsID").toUInt();
        good.name = query.value("GoodsName").toString();
        good.description = query.value("Description").toString();
        good.price = Price(query.value("Price").toDouble());
        good.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            good.image = query.value("Image").toString();
        }
        good.shopId = query.value("ShopID").toUInt();
        good.shopName = query.value("ShopName").toString();
        goods.append(good);
    }
    return goods;
}

QVector<SellerApply> DataInterface::AdminGetAllApplies() {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM SellerApply");
    query.exec();
    QVector<SellerApply> applies;
    while (query.next()) {
        SellerApply apply;
        apply.id = query.value("ApplyID").toUInt();
        apply.userId = query.value("UserID").toUInt();
        apply.phone = query.value("Phone").toString();
        apply.realName = query.value("RealName").toString();
        apply.realIdentityNumber = query.value("IDNumber").toString();
        apply.status = SellerApplyStatus(query.value("Status").toInt());
        apply.time = query.value("ApplyTime").toDateTime();
        applies.append(apply);
    }
    return applies;
}

void DataInterface::reFreshCurrentUserInfo() {
    if (!CurrentUser::getInstance()->isLogin())
        return;
    if (CurrentUser::getInstance()->getUser().isSeller) {
        ID_t id = CurrentUser::getInstance()->getUserId();
        Seller seller = getSellerById(id);
        CurrentUser::getInstance()->setSeller(seller);
    } else {
        ID_t id = CurrentUser::getInstance()->getUserId();
        User user = getUserById(id);
        CurrentUser::getInstance()->setUser(user);
    }
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
        goods.price = Price(query.value("Price").toDouble());
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

QVector<Address> DataInterface::getAddressesByUserId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM Address WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    QVector<Address> addressList;
    while (query.next()) {
        Address address;
        address.id = query.value("AddressID").toUInt();
        address.addressText = query.value("Address").toString();
        address.receiverName = query.value("Receiver").toString();
        address.receiverPhone = query.value("Phone").toString();
        addressList.append(address);
    }
    return addressList;
}

QVector<Comment> DataInterface::getCommentsByGoodsId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM CommentDetail WHERE GoodsID = ?");
    query.addBindValue(id);
    query.exec();
    QVector<Comment> commentList;
    while (query.next()) {
        Comment comment;
        comment.userName = query.value("UserName").toString();
        comment.content = query.value("Content").toString();
        comment.time = query.value("CreateTime").toDateTime();
        commentList.append(comment);
    }
    return commentList;
}

QVector<Goods> DataInterface::getCollectionGoodsByUserId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT GoodsID FROM Collect WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    QVector<Goods> goodsList;
    while (query.next()) {
        ID_t goodsId = query.value("GoodsID").toUInt();
        goodsList.append(getGoodsById(goodsId));
    }
    return goodsList;
}

UserPermission DataInterface::getUserPermissionByUserId(ID_t id) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("SELECT * FROM UserPermission WHERE UserID = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next()) {
        UserPermission permission;
        permission.allowLogin = query.value("AllowLogin").toBool();
        permission.allowShopping = query.value("AllowShopping").toBool();
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
        goodsWithQuantity.goods.price = Price(query.value("Price").toDouble());
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
        goodsWithQuantity.goods.price = Price(query.value("Price").toDouble());
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
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
    }
    QVector<Goods> goodsList;
    while (query.next()) {
        if (query.value("Status").toInt() == 1 || query.value("ShopStatus").toInt() == 1) {
            continue;
        }
        Goods goods;
        goods.id = query.value("GoodsID").toUInt();
        goods.name = query.value("GoodsName").toString();
        goods.description = query.value("Description").toString();
        goods.price = Price(query.value("Price").toDouble());
        goods.status = query.value("Status").toInt();
        if (query.value("Image").isValid()) {
            goods.image = query.value("Image").toString();
        } else {
            goods.image = "";
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

SellerApplySubmitStatus DataInterface::SubmitSellerApply(ID_t userID, const QString &phone, const QString &realName, const QString &realIdentityNumber) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_SellerApply ?, ?, ?, ?");
    query.addBindValue(userID);
    query.addBindValue(phone);
    query.addBindValue(realName);
    query.addBindValue(realIdentityNumber);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return DBError;
    }
    if (query.next()) {
        return Success;
    }
    return AlreadyApplied;
}

std::optional<ID_t> DataInterface::UpdateUser(const User &user) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_UpdateUserInfo ?, ?, ?");
    query.addBindValue(user.id);
    query.addBindValue(user.name);
    query.addBindValue(user.password);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return std::nullopt;
    }
    if (query.next()) {
        query.finish();
        reFreshCurrentUserInfo();
        return query.value("UserID").toUInt();
    }
    return std::nullopt;
}

bool DataInterface::setUserPermissionByUserId(ID_t id, const UserPermission &permission) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE UserPermission SET AllowLogin = ?, AllowShopping = ?, AllowComment = ?, AllowAddShop = ?, AllowAddGoods = ?, AllowHandleOrder = ? WHERE UserID = ?");
    query.addBindValue(permission.allowLogin);
    query.addBindValue(permission.allowShopping);
    query.addBindValue(permission.allowComment);
    query.addBindValue(permission.allowAddShop);
    query.addBindValue(permission.allowAddGoods);
    query.addBindValue(permission.allowHandleOrder);
    query.addBindValue(id);
    return query.exec();
}

bool DataInterface::DeleteAddress(ID_t addressId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("DELETE FROM Address WHERE AddressID = ?");
    query.addBindValue(addressId);
    if (!query.exec()) {
        return false;
    }
    query.finish();
    reFreshCurrentUserInfo();
    return true;
}

bool DataInterface::SetDefaultAddress(ID_t addressId, ID_t userId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_SetDefaultAddress ?, ?");
    query.addBindValue(userId);
    query.addBindValue(addressId);
    if (!query.exec()) {
        return false;
    }
    query.finish();
    reFreshCurrentUserInfo();
    return true;
}

std::optional<ID_t> DataInterface::AddAddress(const QString &addressText, const QString &receiverName, const QString &receiverPhone, ID_t userId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_AddAddress ?, ?, ?, ?");
    query.addBindValue(userId);
    query.addBindValue(receiverName);
    query.addBindValue(receiverPhone);
    query.addBindValue(addressText);
    if (!query.exec()) {
        return std::nullopt;
    }
    if (query.next()) {
        query.finish();
        reFreshCurrentUserInfo();
        return query.value("AddressID").toUInt();
    }
    return std::nullopt;
}

bool DataInterface::CloseShop(ID_t shopId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE Shop SET Status = 1 WHERE ShopID = ?");
    query.addBindValue(shopId);
    return query.exec();
}

bool DataInterface::OpenShop(ID_t shopId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE Shop SET Status = 0 WHERE ShopID = ?");
    query.addBindValue(shopId);
    return query.exec();
}

bool DataInterface::DeActiveGoods(ID_t goodsId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE Goods SET Status = 1 WHERE GoodsID = ?");
    query.addBindValue(goodsId);
    return query.exec();
}

bool DataInterface::ActiveGoods(ID_t goodsId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE Goods SET Status = 0 WHERE GoodsID = ?");
    query.addBindValue(goodsId);
    return query.exec();
}

bool DataInterface::AdminAcceptApply(ID_t applyId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE SellerApply SET Status = 1 WHERE ApplyID = ?");
    query.addBindValue(applyId);
    return query.exec();
}

bool DataInterface::AdminRejectApply(ID_t applyId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE SellerApply SET Status = 2 WHERE ApplyID = ?");
    query.addBindValue(applyId);
    return query.exec();
}

bool DataInterface::AddGoodsToCart(ID_t userId, ID_t goodsId, unsigned int quantity) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_AddGoodsToCart ?, ?, ?");
    query.addBindValue(userId);
    query.addBindValue(goodsId);
    query.addBindValue(quantity);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return false;
    } else {
        return query.next();
    }
}

bool DataInterface::UpdateGoodsQuantityInCart(ID_t userId, ID_t goodsId, unsigned int quantity) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("UPDATE CartGoods SET Quantity = ? WHERE UserID = ? AND GoodsID = ?");
    query.addBindValue(quantity);
    query.addBindValue(userId);
    query.addBindValue(goodsId);
    return query.exec();
}

bool DataInterface::AddGoodsToCollect(ID_t userId, ID_t goodsId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_AddGoodsToCollect ?, ?");
    query.addBindValue(userId);
    query.addBindValue(goodsId);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return false;
    } else {
        return query.next();
    }
}

bool DataInterface::RemoveGoodsFromCollect(ID_t userId, ID_t goodsId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("DELETE FROM Collect WHERE UserID = ? AND GoodsID = ?");
    query.addBindValue(userId);
    query.addBindValue(goodsId);
    return query.exec();
}

bool DataInterface::RemoveGoodsFromCart(ID_t userId, ID_t goodsId) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("DELETE FROM CartGoods WHERE UserID = ? AND GoodsID = ?");
    query.addBindValue(userId);
    query.addBindValue(goodsId);
    return query.exec();
}

bool DataInterface::AddComment(ID_t userId, ID_t goodsId, const QString &content) {
    QSqlQuery query(DBInstance::getInstance());
    query.prepare("EXEC sp_AddComment ?, ?, ?");
    query.addBindValue(goodsId);
    query.addBindValue(userId);
    query.addBindValue(content);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "数据库错误", query.lastError().text());
        return false;
    } else {
        return query.next();
    }
}