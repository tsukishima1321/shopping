-- Description: 创建数据库表和视图

DROP VIEW IF EXISTS UserPermissionDetail;
DROP VIEW IF EXISTS CommentDetail;
DROP VIEW IF EXISTS CartGoodsDetail;
DROP VIEW IF EXISTS OrderGoodsDetail;
DROP VIEW IF EXISTS OrderDetail;
DROP VIEW IF EXISTS ShopDetail;
DROP VIEW IF EXISTS GoodsDetail;


DROP TABLE IF EXISTS UserPermission;
DROP TABLE IF EXISTS Admin;
DROP TABLE IF EXISTS Collect;
DROP TABLE IF EXISTS Comment;
DROP TABLE IF EXISTS CartGoods;
DROP TABLE IF EXISTS OrderGoods;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Goods;
DROP TABLE IF EXISTS Shop;
DROP TABLE IF EXISTS Address;
DROP TABLE IF EXISTS Seller;
DROP TABLE IF EXISTS SellerApply;
DROP TABLE IF EXISTS Users;

CREATE TABLE Users (
    UserID INT PRIMARY KEY IDENTITY(1, 1),
    UserName VARCHAR(20) NOT NULL UNIQUE,
    Password VARCHAR(20) NOT NULL,
    RegisterTime DATETIME NOT NULL,
    LastLoginTime DATETIME NOT NULL,
    IsSeller BIT NOT NULL,
    Nickname VARCHAR(20) NOT NULL
);

CREATE TABLE Seller (
    UserID INT PRIMARY KEY,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    Phone VARCHAR(11) NOT NULL,
    IDNumber VARCHAR(18) NOT NULL,
    RealName VARCHAR(20) NOT NULL
);

CREATE TABLE SellerApply (
    ApplyID INT PRIMARY KEY IDENTITY(1, 1),
    UserID INT,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    Phone VARCHAR(11) NOT NULL,
    IDNumber VARCHAR(18) NOT NULL,
    RealName VARCHAR(20) NOT NULL,
    ApplyTime DATETIME NOT NULL,
    Status INT NOT NULL
);
EXEC sys.sp_addextendedproperty @name = N'MS_Description',
@value = N'0: 未处理，1: 已通过，2: 已拒绝',
@level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'SellerApply', @level2type = N'COLUMN', @level2name = N'Status';
/*
    Status:
    0: 未处理
    1: 已通过
    2: 已拒绝
*/


CREATE TABLE Address (
    AddressID INT PRIMARY KEY IDENTITY(1, 1),
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    Address VARCHAR(100) NOT NULL,
    Receiver VARCHAR(20) NOT NULL,
    Phone VARCHAR(11) NOT NULL,
    IsDefault BIT NOT NULL
);

CREATE TABLE Shop (
    ShopID INT PRIMARY KEY IDENTITY(1, 1),
    SellerID INT NOT NULL,
    FOREIGN KEY (SellerID) REFERENCES Seller(UserID) ON DELETE CASCADE,
    ShopName VARCHAR(20) NOT NULL,
    Description VARCHAR(100) NOT NULL,
    Status INT NOT NULL
);

EXEC sys.sp_addextendedproperty @name = N'MS_Description',
@value = N'0: 正常，1: 关闭',
@level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Shop', @level2type = N'COLUMN', @level2name = N'Status';
/*
    Status:
    0: 正常
    1: 关闭
*/

CREATE TABLE Goods (
    GoodsID INT PRIMARY KEY IDENTITY(1, 1),
    ShopID INT NOT NULL,
    FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) ON DELETE CASCADE,
    GoodsName VARCHAR(40) NOT NULL,
    Description VARCHAR(100) NOT NULL,
    Price DECIMAL(10, 2) NOT NULL,
    Status INT NOT NULL,
    Image VARCHAR(100)
);

EXEC sys.sp_addextendedproperty @name = N'MS_Description',
@value = N'0: 正常，1: 下架',
@level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Goods', @level2type = N'COLUMN', @level2name = N'Status';
/*
    Status:
    0: 正常
    1: 下架
*/
CREATE TABLE Orders (
    OrderID INT PRIMARY KEY IDENTITY(1, 1),
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    ShopID INT NOT NULL,
    FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    AddressID INT,
    FOREIGN KEY (AddressID) REFERENCES Address(AddressID) ON DELETE SET NULL ON UPDATE NO ACTION,
    Status INT NOT NULL,
    TotalPrice DECIMAL(10, 2) NOT NULL,
    CreateTime DATETIME NOT NULL
);

EXEC sys.sp_addextendedproperty @name = N'MS_Description',
@value = N'0: 未处理，1: 已发货，2: 已终止',
@level0type = N'SCHEMA', @level0name = N'dbo', @level1type = N'TABLE', @level1name = N'Orders', @level2type = N'COLUMN', @level2name = N'Status';
/*
    Status:
    0: 未处理
    1: 已发货
    2: 已终止
*/

CREATE TABLE OrderGoods (
    OrderID INT NOT NULL,
    FOREIGN KEY (OrderID) REFERENCES Orders(OrderID) ON DELETE CASCADE,
    GoodsID INT NOT NULL,
    FOREIGN KEY (GoodsID) REFERENCES Goods(GoodsID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    Quantity INT NOT NULL
);

CREATE TABLE CartGoods (
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    GoodsID INT NOT NULL,
    FOREIGN KEY (GoodsID) REFERENCES Goods(GoodsID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    Quantity INT NOT NULL
);

CREATE TABLE Comment (
    CommentID INT PRIMARY KEY IDENTITY(1, 1),
    GoodsID INT NOT NULL,
    FOREIGN KEY (GoodsID) REFERENCES Goods(GoodsID) ON DELETE CASCADE,
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    Content VARCHAR(100) NOT NULL,
    CreateTime DATETIME NOT NULL
);

CREATE TABLE Collect (
    CollectID INT PRIMARY KEY IDENTITY(1, 1),
    GoodsID INT NOT NULL,
    FOREIGN KEY (GoodsID) REFERENCES Goods(GoodsID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    CreateTime DATETIME NOT NULL
);

CREATE TABLE Admin (
    AdminID INT PRIMARY KEY IDENTITY(1, 1),
    AdminName VARCHAR(20) NOT NULL,
    Password VARCHAR(20) NOT NULL
);

CREATE TABLE UserPermission (
    UserID INT PRIMARY KEY,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    AllowLogin BIT NOT NULL,
    AllowShopping BIT NOT NULL,
    AllowComment BIT NOT NULL,
    AllowAddShop BIT NOT NULL,
    AllowAddGoods BIT NOT NULL,
    AllowHandleOrder BIT NOT NULL
);
-- 商家信息视图
CREATE VIEW SellerDetail AS
SELECT Seller.UserID, Users.UserName, Seller.Phone, Seller.IDNumber, Seller.RealName, Users.Nickname, Users.Password
FROM Seller
INNER JOIN Users ON Seller.UserID = Users.UserID;

-- 商品详情视图
CREATE VIEW GoodsDetail AS
SELECT Goods.GoodsID, Goods.GoodsName, Goods.Description, Goods.Price, Goods.Status, Goods.Image, Shop.ShopID, Shop.ShopName, Shop.Status AS ShopStatus
FROM Goods
INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 店铺详情视图
CREATE VIEW ShopDetail AS
SELECT Shop.ShopID, Shop.ShopName, Shop.Description, Shop.Status, Seller.UserID, Users.UserName, Seller.Phone, Seller.IDNumber, Seller.RealName
FROM Shop
INNER JOIN Seller ON Shop.SellerID = Seller.UserID
INNER JOIN Users ON Seller.UserID = Users.UserID;

-- 订单详情视图
DROP VIEW IF EXISTS OrderDetail;
CREATE VIEW OrderDetail AS
SELECT Orders.OrderID, Orders.Status, Orders.TotalPrice, Orders.CreateTime, Users.UserID, Users.UserName, Orders.shopID, Shop.ShopName, Address.Address, Address.Receiver, Address.Phone
FROM Orders
INNER JOIN Users ON Orders.UserID = Users.UserID
INNER JOIN Shop ON Orders.ShopID = Shop.ShopID
INNER JOIN Address ON Orders.AddressID = Address.AddressID;

-- 订单商品详情视图
CREATE VIEW OrderGoodsDetail AS
SELECT OrderGoods.OrderID, OrderGoods.GoodsID, OrderGoods.Quantity, Goods.GoodsName, Goods.Price, Goods.Status, Goods.Image, Shop.ShopName, Shop.ShopID
FROM OrderGoods
INNER JOIN Goods ON OrderGoods.GoodsID = Goods.GoodsID
INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 用户购物车视图
CREATE VIEW CartGoodsDetail AS
SELECT CartGoods.UserID, CartGoods.GoodsID, CartGoods.Quantity, Goods.GoodsName, Goods.Price, Goods.Status, Goods.Image, Shop.ShopName, Shop.ShopID
FROM CartGoods
INNER JOIN Goods ON CartGoods.GoodsID = Goods.GoodsID
INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 用户评论视图
CREATE VIEW CommentDetail AS
SELECT Comment.CommentID, Comment.Content, Comment.CreateTime, Users.Nickname, Comment.GoodsID
FROM Comment
INNER JOIN Users ON Comment.UserID = Users.UserID;

-- 用户权限视图
CREATE VIEW UserPermissionDetail AS
SELECT UserPermission.UserID, Users.UserName, UserPermission.AllowLogin, UserPermission.AllowShopping, UserPermission.AllowComment, UserPermission.AllowAddShop, UserPermission.AllowAddGoods, UserPermission.AllowHandleOrder
FROM UserPermission
INNER JOIN Users ON UserPermission.UserID = Users.UserID;