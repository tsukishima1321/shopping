#pragma once
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QSqlDatabase>
#include <QVector>
#include <optional>

#include "Types.h"

namespace DataInterface {

    void reFreshCurrentUserInfo();

    User getUserById(ID_t id);
    Seller getSellerById(ID_t id);
    Shop getShopById(ID_t id);
    Goods getGoodsById(ID_t id);

    QVector<UserWithTime> AdminGetAllUsers();
    QVector<Seller> AdminGetAllSellers();
    QVector<Shop> AdminGetAllShops();
    QVector<Goods> AdminGetAllGoods();
    QVector<SellerApply> AdminGetAllApplies();

    QVector<Address> getAddressesByUserId(ID_t userId);
    QVector<Shop> getShopsBySellerId(ID_t sellerId);
    QVector<Comment> getCommentsByGoodsId(ID_t goodsId);
    QVector<Goods> getCollectionGoodsByUserId(ID_t id);
    UserPermission getUserPermissionByUserId(ID_t id);

    QVector<Goods> getGoodsByShopId(ID_t shopId);
    QVector<Goods> searchGoodsByName(const QString &name, GoodsOrder order, bool searchName = true, bool searchDescription = false);
    QVector<Shop> searchShopsByName(const QString &name, ShopOrder order, bool searchName = true, bool searchDescription = false);

    QVector<GoodsWithQuantity> getGoodsInUserCart(ID_t userId);
    QVector<GoodsWithQuantity> getGoodsInOrder(ID_t orderId);

    QVector<Order> getOrdersByUserId(ID_t userId);
    QVector<Order> getOrdersBySellerId(ID_t sellerId);

    std::optional<ID_t> UserLogin(const QString &name, const QString &password);
    std::optional<ID_t> AdminLogin(const QString &name, const QString &password);

    std::optional<ID_t> UserRegist(const QString &name, const QString &password);

    SellerApplySubmitStatus SubmitSellerApply(ID_t userID, const QString &phone, const QString &realName, const QString &realIdentityNumber);

    std::optional<ID_t> UpdateUser(const User &user);
    bool setUserPermissionByUserId(ID_t id, const UserPermission &permission);

    bool DeleteAddress(ID_t addressId);
    bool SetDefaultAddress(ID_t addressId, ID_t userId);
    std::optional<ID_t> AddAddress(const QString &addressText, const QString &receiverName, const QString &receiverPhone, ID_t userId);

    bool OpenShop(ID_t shopId);
    bool CloseShop(ID_t shopId);
    bool ActiveGoods(ID_t goodsId);
    bool DeActiveGoods(ID_t goodsId);
    bool AdminAcceptApply(ID_t applyId);
    bool AdminRejectApply(ID_t applyId);

    bool AddGoodsToCart(ID_t userId, ID_t goodsId, unsigned int quantity);
    bool AddGoodsToCollect(ID_t userId, ID_t goodsId);
    bool RemoveGoodsFromCollect(ID_t userId, ID_t goodsId);
    bool RemoveGoodsFromCart(ID_t userId, ID_t goodsId);

    bool UpdateGoodsQuantityInCart(ID_t userId, ID_t goodsId, unsigned int quantity);

    bool AddComment(ID_t userId, ID_t goodsId, const QString &content);

    bool SubmitOrder(ID_t userId, ID_t addressId);
}
