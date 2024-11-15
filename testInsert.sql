-- 插入用户
INSERT INTO Users (UserName, Password, RegisterTime, LastLoginTime, IsSeller)
VALUES ('user1', '123456', GETDATE(), GETDATE(), 0),
       ('seller1', '123456', GETDATE(), GETDATE(), 1),
       ('user2', '123456', GETDATE(), GETDATE(), 0),
       ('user3', '123456', GETDATE(), GETDATE(), 0),
       ('user4', '123', GETDATE(), GETDATE(), 0);

-- 插入商家
INSERT INTO Seller (UserID, Phone, IDNumber, RealName)
VALUES (2, '12345678901', '123456789012345678', '张三');

-- 插入地址
INSERT INTO Address (UserID, Address, Receiver, Phone, IsDefault)
VALUES (1, '广东省广州市天河区', '张三', '12345678901', 1),
       (1, '广东省广州市番禺区', '李四', '12345678902', 0),
       (2, '广东省广州市白云区', '王五', '12345678903', 1),
       (3, '广东省广州市黄埔区', '赵六', '12345678904', 1),
       (4, '广东省广州市越秀区', '孙七', '12345678905', 1);

-- 插入店铺
INSERT INTO Shop (SellerID, ShopName, Description, Status)
VALUES (2, 'shop1', 'shop1 description', 0),
       (2, 'shop2', 'shop2 description', 0),
       (2, 'shop3', 'shop3 description', 0);

-- 插入商品
INSERT INTO Goods (ShopID, GoodsName, Description, Price, Status)
VALUES (1, 'goods1', 'goods1 description', 100, 0),
       (1, 'goods2', 'goods2 description', 200, 0),
       (1, 'goods3', 'goods3 description', 300, 0),
       (2, 'goods4', 'goods4 description', 400, 0),
       (2, 'goods5', 'goods5 description', 500, 0),
       (2, 'goods6', 'goods6 description', 600, 0),
       (3, 'goods7', 'goods7 description', 700, 0),
       (3, 'goods8', 'goods8 description', 800, 0),
       (3, 'goods9', 'goods9 description', 900, 0);

-- 插入管理员
INSERT INTO Admin (AdminName, Password)
VALUES ('admin', 'admin');


