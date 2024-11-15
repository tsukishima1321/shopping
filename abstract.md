**在线购物系统数据库设计(MSSQLServer)**

- 实体信息：
    - 一般用户（User）：用户ID、用户名、密码、注册时间、最后登录时间、是否为商家
    - 商家用户（Seller）：用户ID、手机号、证件号、真实姓名
    - 地址（Address）：地址ID、用户ID、地址、收货人、联系电话、是否默认地址
    - 店铺（Shop）：店铺ID、所属商家ID、店铺名称、店铺描述、店铺状态
    - 商品（Goods）：商品ID、所属店铺ID、商品名称、商品描述、商品价格、商品状态、商品图片
    - 订单（Order）：订单ID、用户ID、订单状态、订单总价、订单创建时间、订单涉及的店铺
    - 订单中的商品（OrderGoods）：订单ID、商品ID、商品数量
    - 用户购物车中的商品（CartGoods）：用户ID、商品ID、商品数量
    - 评论（Comment）：评论ID、商品ID、用户ID、评论内容、评论时间
    - 收藏（Collect）：收藏ID、商品ID、用户ID、收藏时间
    - 管理员（Admin）：管理员ID、管理员名称、密码
    - 用户权限（UserPermission）：用户ID、是否允许登录、是否允许购物、是否允许评论、是否允许添加店铺、是否允许添加商品、是否允许处理订单

- 联系信息：
    - 一般用户与地址：一对多
    - 商家用户为特殊的一般用户，在一般用户的基础上增加了商家信息。
    - 商家用户与店铺：一对多
    - 店铺与商品：一对多
    - 一般用户与订单：一对多
    - 订单与订单中的商品：一对多
    - 用户与用户购物车中的商品：一对多
    - 一般用户与评论：一对多
    - 一般用户与收藏：一对多
    - 用户与用户权限：一对一

- 目标行为：
    - 一般用户：
        - 注册
        - 登录
        - 查看和修改个人信息
        - 查看、添加、删除和修改地址，设置默认地址
        - 查看个人订单内容和状态
        - 查看个人购物车，删除购物车中的商品，修改购物车中的商品数量
        - 将购物车提交为订单
        - 搜索店铺，查看店铺信息和店铺中的商品
        - 搜索商品，查看商品信息和评论
        - 将商品加入收藏
        - 将商品加入购物车
        - 为商品添加评论
        - 认证成为商家

    - 商家用户：
        - 添加、修改和删除店铺
        - 查看个人店铺，查看店铺中的商品
        - 在店铺中添加、删除和修改商品
        - 接收和处理订单

    - 管理员：
        - 查看用户信息
        - 查看店铺信息
        - 调整用户权限，封禁用户

- 行为具体流程：
行为 | 数据库操作
--- | ---
用户注册：用户输入用户名、密码。系统生成用户ID和注册时间，在用户权限表中添加一条记录，设置默认权限。 | INSERT Users, INSERT UserPermission 
用户登录：用户输入用户名、密码。系统验证用户名和密码，返回登录成功或失败，更新用户登录时间。 | SELECT Users 
查看和修改个人信息：用户查看个人信息，可以修改用户名、密码。 | SELECT User，UPDATE Users 
查看、添加、删除和修改地址，设置默认地址：用户查看个人地址，可以添加、删除和修改地址，可以设置默认地址。 | SELECT Address，INSERT Address，DELETE Address，UPDATE Address 
查看个人订单：用户查看个人订单，可以查看订单详情。 | SELECT Orders 
查看个人购物车，删除购物车中的商品，修改购物车中的商品数量：用户查看个人购物车，可以删除购物车中的商品，可以修改购物车中的商品数量。 | SELECT CartGoods，DELETE CartGoods，UPDATE CartGoods 
将购物车提交为订单：用户将购物车中的商品提交为订单，系统生成订单ID和订单创建时间，计算订单总价。 | INSERT Order，DELETE CartGoods，INSERT OrderGoods
搜索店铺，查看店铺信息和店铺中的商品：用户搜索店铺，查看店铺信息和店铺中的商品。 | SELECT Shop，SELECT Goods
搜索商品，查看商品信息和评论：用户搜索商品，查看商品信息和评论。 | SELECT Goods，SELECT Comment
将商品加入收藏：用户将商品加入收藏。 | INSERT Collect
将商品加入购物车：用户将商品加入购物车。 | INSERT CartGoods
为商品添加评论：用户为商品添加评论，系统生成评论ID和评论时间。 | INSERT Comment
认证成为商家：用户申请认证成为商家，系统生成商家用户ID，更新用户权限。 | INSERT Seller，UPDATE UserPermission
添加、修改和删除店铺：商家用户添加、修改和删除店铺。 | INSERT Shop，UPDATE Shop，DELETE Shop
查看个人店铺，查看店铺中的商品：商家用户查看个人店铺，查看店铺中的商品。 | SELECT Shop，SELECT Goods
在店铺中添加、删除和修改商品：商家用户在店铺中添加、删除和修改商品。 | INSERT Goods，DELETE Goods，UPDATE Goods
接收和处理订单：商家用户接收和处理订单。 | SELECT Order，UPDATE Orders
查看用户信息：管理员查看用户信息。 | SELECT Users
查看店铺信息：管理员查看店铺信息。 | SELECT Shop
调整用户权限，封禁用户：管理员调整用户权限，封禁用户。 | UPDATE UserPermission

- SQL建立表格：
```sql
CREATE TABLE Users (
    UserID INT PRIMARY KEY IDENTITY(1, 1),
    UserName VARCHAR(20) NOT NULL,
    Password VARCHAR(20) NOT NULL,
    RegisterTime DATETIME NOT NULL,
    LastLoginTime DATETIME NOT NULL,
    IsSeller BIT NOT NULL
);

CREATE TABLE Seller (
    UserID INT PRIMARY KEY,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE CASCADE,
    Phone VARCHAR(11) NOT NULL,
    IDNumber VARCHAR(18) NOT NULL,
    RealName VARCHAR(20) NOT NULL
);

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

CREATE TABLE Goods (
    GoodsID INT PRIMARY KEY IDENTITY(1, 1),
    ShopID INT NOT NULL,
    FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) ON DELETE CASCADE,
    GoodsName VARCHAR(20) NOT NULL,
    Description VARCHAR(100) NOT NULL,
    Price DECIMAL(10, 2) NOT NULL,
    Status INT NOT NULL,
    Image VARCHAR(100)
);

CREATE TABLE Orders (
    OrderID INT PRIMARY KEY IDENTITY(1, 1),
    UserID INT NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    ShopID INT NOT NULL,
    FOREIGN KEY (ShopID) REFERENCES Shop(ShopID) ON DELETE NO ACTION ON UPDATE NO ACTION,
    Status INT NOT NULL,
    TotalPrice DECIMAL(10, 2) NOT NULL,
    CreateTime DATETIME NOT NULL
);

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
```

- 视图设计：
```sql

-- 商品详情视图
CREATE VIEW GoodsDetail AS
SELECT Goods.GoodsID, Goods.GoodsName, Goods.Description, Goods.Price, Goods.Status, Goods.Image, Shop.ShopID, Shop.ShopName, Shop.Description AS ShopDescription, Shop.Status AS ShopStatus
FROM Goods
INNER INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 店铺详情视图
CREATE VIEW ShopDetail AS
SELECT Shop.ShopID, Shop.ShopName, Shop.Description, Shop.Status, Seller.UserID, Seller.Phone, Seller.IDNumber, Seller.RealName
FROM Shop
INNER JOIN Seller ON Shop.SellerID = Seller.UserID;

-- 订单详情视图
CREATE VIEW OrderDetail AS
SELECT Orders.OrderID, Orders.Status, Orders.TotalPrice, Orders.CreateTime, Users.UserID, Users.UserName
FROM Orders
INNER JOIN Users ON Orders.UserID = Users.UserID;

-- 订单商品详情视图
CREATE VIEW OrderGoodsDetail AS
SELECT OrderGoods.OrderID, OrderGoods.GoodsID, OrderGoods.Quantity, Goods.GoodsName, Goods.Price, Goods.Image, Shop.ShopName
FROM OrderGoods
INNER JOIN Goods ON OrderGoods.GoodsID = Goods.GoodsID
INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 用户购物车视图
CREATE VIEW CartGoodsDetail AS
SELECT CartGoods.UserID, CartGoods.GoodsID, CartGoods.Quantity, Goods.GoodsName, Goods.Price, Goods.Image, Shop.ShopName
FROM CartGoods
INNER JOIN Goods ON CartGoods.GoodsID = Goods.GoodsID
INNER JOIN Shop ON Goods.ShopID = Shop.ShopID;

-- 用户评论视图
CREATE VIEW CommentDetail AS
SELECT Comment.CommentID, Comment.Content, Comment.CreateTime, Users.UserName
FROM Comment
INNER JOIN Users ON Comment.UserID = Users.UserID;

-- 用户权限视图
CREATE VIEW UserPermissionDetail AS
SELECT UserPermission.UserID, Users.UserName, UserPermission.AllowLogin, UserPermission.AllowShopping, UserPermission.AllowComment, UserPermission.AllowAddShop, UserPermission.AllowAddGoods, UserPermission.AllowHandleOrder
FROM UserPermission
INNER JOIN Users ON UserPermission.UserID = Users.UserID;

```

- 触发器设计（MS SQLServer）：
```sql
-- 用户注册时，自动添加用户权限
DROP TRIGGER IF EXISTS tr_UserCreate
CREATE TRIGGER tr_UserCreate ON Users FOR INSERT
AS
-- Usu curser to iterate through the inserted table
DECLARE insert_cursor CURSOR FOR SELECT UserID FROM inserted
DECLARE @UserID INT
OPEN insert_cursor
FETCH NEXT FROM insert_cursor INTO @UserID
WHILE @@FETCH_STATUS = 0
BEGIN
    INSERT INTO UserPermission (UserID, AllowLogin, AllowShopping, AllowComment, AllowAddShop, AllowAddGoods, AllowHandleOrder)
    VALUES (@UserID, 1, 1, 1, 0, 0, 0);
    FETCH NEXT FROM insert_cursor INTO @UserID
END

-- 用户认证成为商家时，自动更新用户权限
CREATE TRIGGER tr_UserAuthSeller ON Seller FOR INSERT
AS
DECLARE insert_cursor CURSOR FOR SELECT UserID FROM inserted
DECLARE @UserID INT
OPEN insert_cursor
FETCH NEXT FROM insert_cursor INTO @UserID
WHILE @@FETCH_STATUS = 0
BEGIN
    UPDATE UserPermission
    SET AllowAddShop = 1, AllowAddGoods = 1, AllowHandleOrder = 1
    WHERE UserID = @UserID;
    FETCH NEXT FROM insert_cursor INTO @UserID
END

--
```

- 储存过程设计（MS SQLServer）：
```sql
-- 用户登录
CREATE PROCEDURE sp_UserLogin
    @UserName VARCHAR(20),
    @Password VARCHAR(20)
AS
BEGIN
    SELECT UserID FROM Users
    WHERE UserName = @UserName AND Password = @Password;
    -- 如果登录成功，更新最后登录时间
    IF @@ROWCOUNT > 0
    BEGIN
        UPDATE Users
        SET LastLoginTime = GETDATE()
        WHERE UserName = @UserName;
    END
END;

-- 设置默认地址
CREATE PROCEDURE sp_SetDefaultAddress
    @UserID INT,
    @AddressID INT
AS
BEGIN
    UPDATE Address
    SET IsDefault = 0
    WHERE UserID = @UserID;
    UPDATE Address
    SET IsDefault = 1
    WHERE AddressID = @AddressID;
END;

-- 用户将商品加入购物车
CREATE PROCEDURE sp_AddToCart
    @UserID INT,
    @GoodsID INT,
    @Quantity INT
AS
BEGIN
    INSERT INTO CartGoods (UserID, GoodsID, Quantity)
    VALUES (@UserID, @GoodsID, @Quantity);
END;

-- 用户收藏商品
CREATE PROCEDURE sp_AddToCollect
    @UserID INT,
    @GoodsID INT
AS
BEGIN
    INSERT INTO Collect (UserID, GoodsID, CreateTime)
    VALUES (@UserID, @GoodsID, GETDATE());
END;

-- 将购物车提交为订单
DROP PROCEDURE IF EXISTS sp_SubmitOrder;
CREATE PROCEDURE sp_SubmitOrder
    @UserID INT
AS
BEGIN
    DECLARE @OrderID INT;
    DECLARE @ShopID INT;
    DECLARE @CreateTime DATETIME;
    DECLARE @GoodsID INT;
    DECLARE @Quantity INT;
    DECLARE @Price DECIMAL(10, 2);
    DECLARE @ShopTotalPrice DECIMAL(10, 2) = 0;
    DECLARE @ShopGoods TABLE (ShopID INT, GoodsID INT, Quantity INT, Price DECIMAL(10, 2));
    DECLARE CartCursor CURSOR FOR SELECT GoodsID, Quantity FROM CartGoods WHERE UserID = @UserID;
    OPEN CartCursor;
	FETCH NEXT FROM CartCursor INTO @GoodsID, @Quantity;
    WHILE @@FETCH_STATUS = 0
    BEGIN
        SELECT @ShopID = ShopID FROM Goods WHERE GoodsID = @GoodsID;
        SELECT @Price = Price FROM Goods WHERE GoodsID = @GoodsID;
        INSERT INTO @ShopGoods VALUES (@ShopID, @GoodsID, @Quantity, @Price);
        FETCH NEXT FROM CartCursor INTO @GoodsID, @Quantity;
    END
    CLOSE CartCursor;
    DEALLOCATE CartCursor;
    DECLARE ShopCursor CURSOR FOR SELECT DISTINCT ShopID FROM @ShopGoods;
    OPEN ShopCursor;
    FETCH NEXT FROM ShopCursor INTO @ShopID;
    WHILE @@FETCH_STATUS = 0
    BEGIN
        SELECT @ShopTotalPrice = 0;
        DECLARE ShopGoodsCursor CURSOR FOR SELECT GoodsID, Quantity, Price FROM @ShopGoods WHERE ShopID = @ShopID;
        OPEN ShopGoodsCursor;
        FETCH NEXT FROM ShopGoodsCursor INTO @GoodsID, @Quantity, @Price;
        WHILE @@FETCH_STATUS = 0
        BEGIN
            SELECT @ShopTotalPrice = @ShopTotalPrice + @Price*@Quantity;
            FETCH NEXT FROM ShopGoodsCursor INTO @GoodsID, @Quantity, @Price;
        END
        CLOSE ShopGoodsCursor;
        DEALLOCATE ShopGoodsCursor;
        INSERT INTO Orders (UserID, ShopID, Status, TotalPrice, CreateTime)
        VALUES (@UserID, @ShopID, 0, @ShopTotalPrice, GETDATE());
        SELECT @OrderID = SCOPE_IDENTITY();
        DECLARE OrderGoodsCursor CURSOR FOR SELECT GoodsID, Quantity FROM @ShopGoods WHERE ShopID = @ShopID;
        OPEN OrderGoodsCursor;
        FETCH NEXT FROM OrderGoodsCursor INTO @GoodsID, @Quantity;
        WHILE @@FETCH_STATUS = 0
        BEGIN
            INSERT INTO OrderGoods VALUES (@OrderID, @GoodsID, @Quantity);
            FETCH NEXT FROM OrderGoodsCursor INTO @GoodsID, @Quantity;
        END
        CLOSE OrderGoodsCursor;
        DEALLOCATE OrderGoodsCursor;
        FETCH NEXT FROM ShopCursor INTO @ShopID;
    END
    DELETE FROM CartGoods WHERE UserID = @UserID;
END;

-- 为商品添加评论
CREATE PROCEDURE sp_AddComment
    @GoodsID INT,
    @UserID INT,
    @Content VARCHAR(100)
AS
BEGIN
    INSERT INTO Comment (GoodsID, UserID, Content, CreateTime)
    VALUES (@GoodsID, @UserID, @Content, GETDATE());
END;

-- 商家查询与自己相关的订单
CREATE PROCEDURE sp_QueryOrder
    @SellerID INT
AS
BEGIN
    SELECT Orders.OrderID
    FROM Orders
    INNER JOIN Users ON Orders.UserID = Users.UserID
    WHERE Orders.ShopID IN (SELECT ShopID FROM Shop WHERE SellerID = @SellerID);
END;
```
- 为数据库插入测试数据： 
```sql
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
VALUES (2, 'shop1', 'shop1 description', 1),
       (2, 'shop2', 'shop2 description', 1),
       (2, 'shop3', 'shop3 description', 1);

-- 插入商品
INSERT INTO Goods (ShopID, GoodsName, Description, Price, Status)
VALUES (1, 'goods1', 'goods1 description', 100, 1),
       (1, 'goods2', 'goods2 description', 200, 1),
       (1, 'goods3', 'goods3 description', 300, 1),
       (2, 'goods4', 'goods4 description', 400, 1),
       (2, 'goods5', 'goods5 description', 500, 1),
       (2, 'goods6', 'goods6 description', 600, 1),
       (3, 'goods7', 'goods7 description', 700, 1),
       (3, 'goods8', 'goods8 description', 800, 1),
       (3, 'goods9', 'goods9 description', 900, 1);

-- 插入管理员
INSERT INTO Admin (AdminName, Password)
VALUES ('admin', 'admin');

```

- 初步测试语句
```sql

-- 用户登录
EXEC sp_UserLogin 'user1', '123456';

-- 设置默认地址
EXEC sp_SetDefaultAddress 7, 8;

-- 用户将商品加入购物车
INSERT INTO CartGoods (UserID, GoodsID, Quantity)
VALUES (7, 12, 1),
       (7, 13, 2),
       (7, 15, 3);

-- 用户将购物车提交为订单
EXEC sp_SubmitOrder 7;

-- 商家处理订单
-- 从储存过程中获取订单ID
DECLARE @Table TABLE (OrderID INT);
INSERT INTO @Table
EXEC sp_QueryOrder 2;
SELECT * FROM @Table;
UPDATE Orders
SET Status = 1
WHERE OrderID = (SELECT TOP 1 OrderID FROM @Table);
```

```

- 客户端窗体设计：
    - 登录窗体
        - 选择是否为管理员
        - 输入用户名、密码
        - 登录按钮
        - 注册按钮
        - 提示登录成功或失败
        - 若登录成功，关闭登录窗体，打开用户主窗体或管理员主窗体（记录数据库返回的用户ID）
    - 注册窗体
        - 输入用户名、密码
        - 注册按钮
        - 提示注册成功或失败
        - 若注册成功，关闭注册窗体，回到登录窗体
    - 用户主窗体
        - 打开搜索窗体按钮
        - 显示用户基本信息
        - 打开用户信息和地址窗体按钮
        - 打开用户订单窗体按钮
        - 打开用户购物车窗体按钮
        - 打开用户收藏窗体按钮
        - 注册成为商家按钮
    - 用户信息和地址窗体
        - 显示用户信息
        - 修改用户名、密码按钮
        - 显示用户地址
        - 添加地址按钮
        - 删除地址按钮
        - 修改地址按钮
        - 设置默认地址按钮
    - 用户收藏窗体
        - 显示用户收藏的商品
        - 删除收藏按钮
    - 注册成为商家窗体
        - 输入手机号、证件号、真实姓名
        - 提交按钮
        - 提示提交成功或失败
        - 若提交成功，关闭窗体
    - 用户订单窗体
        - 显示用户订单和状态
        - 订单下拉显示订单包含的商品
    - 用户购物车窗体
        - 打开订单提交窗体按钮
        - 显示购物车中的商品
        - 删除商品按钮
        - 修改商品数量按钮
    - 订单提交窗体
        - 显示订单总价
        - 选择地址
        - 提交订单按钮
    - 搜索窗体
        - 输入搜索关键字
        - 搜索条件：搜索店铺还是商品、排序方式、过滤器等
        - 搜索按钮
        - 显示搜索结果
        - 点击店铺显示店铺详情窗体
        - 点击商品显示商品详情窗体
    - 店铺详情窗体
        - 显示店铺信息
        - 显示店铺中的商品
        - 点击商品显示商品详情窗体
    - 商品详情窗体
        - 显示商品信息
        - 显示商品评论
        - 添加评论按钮
        - 加入购物车按钮
        - 加入收藏按钮
    - 商家主窗体
        - 显示商家信息
        - 显示商家店铺
        - 添加店铺按钮
        - 删除店铺按钮
        - 点击店铺显示店铺管理窗体
        - 打开订单处理窗体按钮
    - 订单处理窗体
        - 显示订单信息
        - 修改订单状态按钮
    - 店铺管理窗体
        - 显示店铺信息
        - 显示店铺中的商品
        - 添加商品按钮
        - 删除商品按钮
        - 点击商品显示商品管理
    - 商品管理窗体
        - 显示商品信息
        - 修改商品信息按钮
        - 删除商品按钮
    - 管理员主窗体
        - 浏览用户列表
        - 点击用户显示用户权限管理窗体
    - 用户权限管理窗体
        - 显示用户权限
        - 修改用户权限按钮
        - 封禁用户按钮



