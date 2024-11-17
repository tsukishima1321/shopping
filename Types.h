#pragma once

#include <QString>
#include <QDateTime>

using ID_t = unsigned int;

class Price {
public:
    explicit Price(unsigned int price = 0.0) :
            price(price) {}
    Price(const Price &price) :
            price(price.price) {}
    Price &operator=(const Price &price) {
        this->price = price.price;
        return *this;
    }
    explicit Price(double price) {
        this->price = price * 100;
    }
    Price operator+(const Price &price) const {
        return Price(this->price + price.price);
    }
    Price operator-(const Price &price) const {
        return Price(this->price - price.price);
    }
    Price operator*(unsigned int x) const {
        return Price(this->price * x);
    }
    explicit operator double() const {
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
    Address *defaultAddress;
    virtual ~User() = default;
};

struct UserWithTime {
    User user;
    QDateTime registerTime;
    QDateTime lastLoginTime;
};

struct Seller : public User {
    QString phone;
    QString realIdentityNumber;
    QString realName;
};

struct Shop {
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

enum SellerApplyStatus {
    Pending = 0,
    Accept = 1,
    Reject = 2
};

struct SellerApply {
    ID_t id;
    ID_t userId;
    QString phone;
    QString realName;
    QString realIdentityNumber;
    QDateTime time;
    SellerApplyStatus status;
};

struct UserPermission {
    bool allowLogin;
    bool allowShopping;
    bool allowComment;
    bool allowAddShop;
    bool allowAddGoods;
    bool allowHandleOrder;
};

struct Comment {
    QString userName;
    QString content;
    QDateTime time;
};

struct Order{
    ID_t orderId;
    ID_t shopId;
    ID_t userId;
    QString shopName;
    int status;
    QDateTime time;
    QVector<GoodsWithQuantity> goods;
};

enum GoodsOrder {
    GoodsPriceAscending,
    GoodsPriceDescending,
    GoodsNameAscending,
    GoodsNameDescending
};

enum ShopOrder {
    ShopNameAscending,
    ShopNameDescending
};

enum SellerApplySubmitStatus {
    Success,
    AlreadyApplied,
    DBError
};