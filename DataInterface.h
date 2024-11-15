#pragma once
#include <QList>
#include <QMap>
#include <QString>
#include <QVector>
#include <QSqlDatabase>
#include <QDateTime>
#include <optional>

using ID_t = unsigned int;

class Price {
public:
    explicit Price(unsigned int price = 0.0) : price(price){}
    Price(const Price &price) : price(price.price){}
    Price& operator=(const Price &price){
        this->price = price.price;
        return *this;
    }
    Price(double price){
        this->price = price * 100;
    }
    Price operator+(const Price &price) const {
        return this->price + price.price;
    }
    Price operator-(const Price &price) const {
        return this->price - price.price;
    }
    Price operator*(unsigned int x) const {
        return this->price * x;
    }
    operator double() const {
        return price / 100.0;
    }
    QString toString() const {
        return QString::number(price / 100) + "." + QString::number(price % 100);
    }
private:
    unsigned int price;
};

struct Address {
    ID_t id;
    QString addressText;
    QString receiverName;
    QString receiverPhone;
};

struct User {
    ID_t id;
    QString name;
    QString password;
    bool isSeller;
    QList<Address> addresses;
    Address* defaultAddress;
};

struct Seller : public User {
    QString phone;
    QString realIdentityNumber;
    QString realName;
};

struct Shop{
    ID_t id;
    QString name;
    QString description;
    ID_t sellerId;
    QString sellerName;
    int status;
};

struct Goods {
    ID_t id;
    QString name;
    QString description;
    Price price;
    int status;
    QString image;
    ID_t shopId;
    QString shopName;
};

struct GoodsWithQuantity {
    Goods goods;
    unsigned int quantity;
    Price totalPrice;
};

struct UserPermission {
    bool allowLogin;
    bool AllowShopping;
    bool allowComment;
    bool allowAddShop;
    bool allowAddGoods;
    bool allowHandleOrder;
};

struct Comment {
    ID_t id;
    ID_t goodsId;
    ID_t userId;
    QString userName;
    QString content;
    QDateTime time;
};

enum GoodsOrder{
    GoodsPriceAscending,
    GoodsPriceDescending,
    GoodsNameAscending,
    GoodsNameDescending
};

enum ShopOrder{
    ShopNameAscending,
    ShopNameDescending
};

namespace DataInterface{
    User getUserById(ID_t id);
    Seller getSellerById(ID_t id);
    Shop getShopById(ID_t id);
    Goods getGoodsById(ID_t id);

    QVector<Shop> getShopsBySellerId(ID_t sellerId);
    QVector<Comment> getCommentsByGoodsId(ID_t goodsId);
    UserPermission getUserPermissionByUserId(ID_t id);

    QVector<Goods> getGoodsByShopId(ID_t shopId);
    QVector<Goods> searchGoodsByName(const QString &name, GoodsOrder order, bool searchName = true, bool searchDescription = false);
    QVector<Shop> searchShopsByName(const QString &name, ShopOrder order, bool searchName = true, bool searchDescription = false);

    QVector<GoodsWithQuantity> getGoodsInUserCart(ID_t userId);
    QVector<GoodsWithQuantity> getGoodsInOrder(ID_t orderId);

    std::optional<ID_t> UserLogin(const QString &name, const QString &password);
    std::optional<ID_t> AdminLogin(const QString &name, const QString &password);

    std::optional<ID_t> UserRegist(const QString &name, const QString &password);
}

