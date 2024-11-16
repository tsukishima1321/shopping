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
    virtual ~User() = default;
};

struct UserWithTime{
    User user;
    QDateTime registerTime;
    QDateTime lastLoginTime;
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

enum SellerApplySubmitStatus{
    Success,
    AlreadyApplied,
    DBError
};

namespace DataInterface{

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
    UserPermission getUserPermissionByUserId(ID_t id);

    QVector<Goods> getGoodsByShopId(ID_t shopId);
    QVector<Goods> searchGoodsByName(const QString &name, GoodsOrder order, bool searchName = true, bool searchDescription = false);
    QVector<Shop> searchShopsByName(const QString &name, ShopOrder order, bool searchName = true, bool searchDescription = false);

    QVector<GoodsWithQuantity> getGoodsInUserCart(ID_t userId);
    QVector<GoodsWithQuantity> getGoodsInOrder(ID_t orderId);

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
}

