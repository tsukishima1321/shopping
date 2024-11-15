-- Description: 创建存储过程

-- 用户注册
DROP PROCEDURE IF EXISTS sp_UserRegist;
CREATE PROCEDURE sp_UserRegist
    @UserName VARCHAR(20),
    @Password VARCHAR(20)
AS
BEGIN
-- 不允许用户名重复
    IF EXISTS (SELECT UserID FROM Users WHERE UserName = @UserName)
    BEGIN
        RETURN;
    END
    INSERT INTO Users (UserName, Password, RegisterTime, LastLoginTime, isSeller)
    VALUES (@UserName, @Password, GETDATE(), GETDATE(), 0);
    SELECT SCOPE_IDENTITY();
END

-- 用户登录
DROP PROCEDURE IF EXISTS sp_UserLogin;
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
DROP PROCEDURE IF EXISTS sp_SetDefaultAddress;
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
DROP PROCEDURE IF EXISTS sp_AddToCart;
CREATE PROCEDURE sp_AddToCart
    @UserID INT,
    @GoodsID INT,
    @Quantity INT
AS
BEGIN
    INSERT INTO CartGoods (UserID, GoodsID, Quantity)
    VALUES (@UserID, @GoodsID, @Quantity);
    SELECT SCOPE_IDENTITY();
END;

-- 用户收藏商品
DROP PROCEDURE IF EXISTS sp_AddToCollect;
CREATE PROCEDURE sp_AddToCollect
    @UserID INT,
    @GoodsID INT
AS
BEGIN
    INSERT INTO Collect (UserID, GoodsID, CreateTime)
    VALUES (@UserID, @GoodsID, GETDATE());
    SELECT SCOPE_IDENTITY();
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
    DECLARE CartCursor CURSOR LOCAL FOR SELECT GoodsID, Quantity FROM CartGoods WHERE UserID = @UserID;
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
    DECLARE ShopCursor CURSOR LOCAL FOR SELECT DISTINCT ShopID FROM @ShopGoods;
    OPEN ShopCursor;
    FETCH NEXT FROM ShopCursor INTO @ShopID;
    WHILE @@FETCH_STATUS = 0
    BEGIN
        SELECT @ShopTotalPrice = 0;
        DECLARE ShopGoodsCursor CURSOR LOCAL FOR SELECT GoodsID, Quantity, Price FROM @ShopGoods WHERE ShopID = @ShopID;
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
        DECLARE OrderGoodsCursor CURSOR LOCAL FOR SELECT GoodsID, Quantity FROM @ShopGoods WHERE ShopID = @ShopID;
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
DROP PROCEDURE IF EXISTS sp_AddComment;
CREATE PROCEDURE sp_AddComment
    @GoodsID INT,
    @UserID INT,
    @Content VARCHAR(100)
AS
BEGIN
    INSERT INTO Comment (GoodsID, UserID, Content, CreateTime)
    VALUES (@GoodsID, @UserID, @Content, GETDATE());
    SELECT SCOPE_IDENTITY();
END;

-- 商家新开店铺
DROP PROCEDURE IF EXISTS sp_AddShop;
CREATE PROCEDURE sp_AddShop
    @SellerID INT,
    @ShopName VARCHAR(20),
    @Description VARCHAR(100)
AS
BEGIN
    INSERT INTO Shop (SellerID, ShopName, Description, Status)
    VALUES (@SellerID, @ShopName, @Description, 1);
    SELECT SCOPE_IDENTITY();
END;

-- 激活店铺
DROP PROCEDURE IF EXISTS sp_ActivateShop;
CREATE PROCEDURE sp_ActivateShop
    @ShopID INT
AS
BEGIN
    UPDATE Shop
    SET Status = 1
    WHERE ShopID = @ShopID;
END;

-- 关闭店铺
DROP PROCEDURE IF EXISTS sp_DeactivateShop;
CREATE PROCEDURE sp_DeactivateShop
    @ShopID INT
AS
BEGIN
    UPDATE Shop
    SET Status = 0
    WHERE ShopID = @ShopID;
END;

-- 商家添加商品
DROP PROCEDURE IF EXISTS sp_AddGoods;
CREATE PROCEDURE sp_AddGoods
    @ShopID INT,
    @GoodsName VARCHAR(20),
    @Description VARCHAR(100),
    @Price DECIMAL(10, 2)
AS
BEGIN
    INSERT INTO Goods (ShopID, GoodsName, Description, Price, Status)
    VALUES (@ShopID, @GoodsName, @Description, @Price, 0);
    SELECT SCOPE_IDENTITY();
END;

-- 下架商品
DROP PROCEDURE IF EXISTS sp_DeactiveGoods;
CREATE PROCEDURE sp_DeactiveGoods
    @GoodsID INT
AS
BEGIN
    UPDATE Goods
    SET Status = 1
    WHERE GoodsID = @GoodsID;
END;

-- 上架商品
DROP PROCEDURE IF EXISTS sp_ActiveGoods;
CREATE PROCEDURE sp_ActiveGoods
    @GoodsID INT
AS
BEGIN
    UPDATE Goods
    SET Status = 0
    WHERE GoodsID = @GoodsID;
END;

-- 商家查询与自己相关的订单
DROP PROCEDURE IF EXISTS sp_QueryOrder;
CREATE PROCEDURE sp_QueryOrder
    @SellerID INT
AS
BEGIN
    SELECT Orders.OrderID
    FROM Orders
    INNER JOIN Users ON Orders.UserID = Users.UserID
    WHERE Orders.ShopID IN (SELECT ShopID FROM Shop WHERE SellerID = @SellerID);
END;