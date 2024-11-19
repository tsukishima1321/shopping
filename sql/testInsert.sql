-- 插入用户
INSERT INTO Users (UserName, Password, RegisterTime, LastLoginTime, IsSeller)
VALUES ('user1', '123456', GETDATE(), GETDATE(), 0),
       ('seller1', '123456', GETDATE(), GETDATE(), 1),
       ('seller2', '123456', GETDATE(), GETDATE(), 1),
       ('seller3', '123456', GETDATE(), GETDATE(), 1),
       ('user2', '123456', GETDATE(), GETDATE(), 0),
       ('user3', '123456', GETDATE(), GETDATE(), 0),
       ('user4', '123', GETDATE(), GETDATE(), 0);

-- 插入商家
INSERT INTO Seller (UserID, Phone, IDNumber, RealName)
VALUES (2, '12345678901', '123456789012345678', '张三'),
       (3, '12345678902', '123456789012345679', '李四'),
       (4, '12345678903', '123456789012345670', '王五');

-- 插入地址
INSERT INTO Address (UserID, Address, Receiver, Phone, IsDefault)
VALUES (1, '广东省广州市天河区', '王小明', '12345678901', 1),
       (1, '广东省广州市番禺区', '王小红', '12345678902', 0),
       (2, '广东省广州市白云区', '张三', '12345678903', 1),
       (3, '广东省广州市黄埔区', '李四', '12345678904', 1),
       (4, '广东省广州市越秀区', '王五', '12345678905', 1),
       (5, '广东省广州市荔湾区', '陈大海', '12345678906', 1),
       (5, '广东省广州市花都区', '陈小明', '12345678907', 0),
       (6, '广东省广州市海珠区', '黄小华', '12345678907', 1),
       (7, '广东省广州市南沙区', '马小丽', '12345678908', 1);

-- 插入店铺
INSERT INTO Shop (SellerID, ShopName, Description, Status)
VALUES (2, 'shop1', 'shop1 description', 0),
       (2, 'shop2', 'shop2 description', 0),
       (2, 'shop3', 'shop3 description', 0),
       (3, 'shop4', 'shop4 description', 0),
       (3, 'shop5', 'shop5 description', 0),
       (3, 'shop6', 'shop6 description', 0),
       (4, 'shop7', 'shop7 description', 0),
       (4, 'shop8', 'shop8 description', 0);

-- 插入商品
INSERT INTO Goods (ShopID, GoodsName, Description, Price, Status)
VALUES (1, 'goods1', 'goods1 description', 100, 0),
       (1, 'goods2', 'goods2 description', 200, 0),
       (1, 'goods3', 'goods3 description', 300, 0),
       (2, 'goods4', 'goods4 description', 400, 0),
       (2, 'goods5', 'goods5 description', 59.98, 0),
       (2, 'goods6', 'goods6 description', 39.99, 0),
       (3, 'goods7', 'goods7 description', 99.99, 0),
       (3, 'goods8', 'goods8 description', 666, 0),
       (3, 'goods9', 'goods9 description', 39.95, 0),
       (4, 'goods10', 'goods10 description', 200, 0),
       (4, 'goods11', 'goods11 description', 110, 0),
       (4, 'goods12', 'goods12 description', 130, 0),
       (5, 'goods13', 'goods13 description', 400, 0),
       (5, 'goods14', 'goods14 description', 59.98, 0),
       (5, 'goods15', 'goods15 description', 39.99, 0),
       (6, 'goods16', 'goods16 description', 99.99, 0),
       (6, 'goods17', 'goods17 description', 666, 0),
       (6, 'goods18', 'goods18 description', 39.95, 0),
       (7, 'goods19', 'goods19 description', 200, 0),
       (7, 'goods20', 'goods20 description', 110, 0),
       (7, 'goods21', 'goods21 description', 130, 0),
       (8, 'goods22', 'goods22 description', 400, 0),
       (8, 'goods23', 'goods23 description', 59.98, 0),
       (8, 'goods24', 'goods24 description', 39.99, 0);

-- 插入管理员
INSERT INTO Admin (AdminName, Password)
VALUES ('admin', 'admin');


