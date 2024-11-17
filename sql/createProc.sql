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
    DECLARE @AllowLogin BIT;
    IF EXISTS (SELECT UserID FROM Users WHERE UserName = @UserName AND Password = @Password)
    BEGIN
        SELECT @AllowLogin = AllowLogin FROM UserPermissionDetail WHERE UserName = @UserName;
        IF @AllowLogin = 0
        BEGIN
            RETURN;
        END
        UPDATE Users
        SET LastLoginTime = GETDATE()
        WHERE UserName = @UserName;
        SELECT UserID FROM Users WHERE UserName = @UserName AND Password = @Password;
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
    @UserID INT,
    @AddressID INT
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
        INSERT INTO Orders (UserID, ShopID, Status, TotalPrice, CreateTime, AddressID)
        VALUES (@UserID, @ShopID, 0, @ShopTotalPrice, GETDATE(), @AddressID);
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
    DECLARE @AllowComment BIT;
    SELECT @AllowComment = AllowComment FROM UserPermissionDetail WHERE UserID = @UserID;
    IF @AllowComment = 0
    BEGIN
        RETURN;
    END
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
    @Price DECIMAL(10, 2),
    @Image VARCHAR(100)
AS
BEGIN
    INSERT INTO Goods (ShopID, GoodsName, Description, Price, Status, Image)
    VALUES (@ShopID, @GoodsName, @Description, @Price, 0, @Image);
    SELECT SCOPE_IDENTITY();
END;

-- 更新商品
DROP PROCEDURE IF EXISTS sp_UpdateGoods;
CREATE PROCEDURE sp_UpdateGoods
    @GoodsID INT,
    @GoodsName VARCHAR(20),
    @Description VARCHAR(100),
    @Price DECIMAL(10, 2),
    @Image VARCHAR(100)
AS
BEGIN
    UPDATE Goods
    SET GoodsName = @GoodsName, Description = @Description, Price = @Price, Image = @Image
    WHERE GoodsID = @GoodsID;
    SELECT @GoodsID;
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

-- 添加店铺
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

-- 更新店铺
DROP PROCEDURE IF EXISTS sp_UpdateShop;
CREATE PROCEDURE sp_UpdateShop
    @ShopID INT,
    @ShopName VARCHAR(20),
    @Description VARCHAR(100)
AS
BEGIN
    UPDATE Shop
    SET ShopName = @ShopName, Description = @Description
    WHERE ShopID = @ShopID;
    SELECT @ShopID;
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

-- 用户提交商家申请
DROP PROCEDURE IF EXISTS sp_SellerApply;
CREATE PROCEDURE sp_SellerApply
    @UserID INT,
    @Phone VARCHAR(20),
    @RealName VARCHAR(20),
    @IDCard VARCHAR(20)
AS
BEGIN
    IF EXISTS (SELECT UserID FROM SellerApply WHERE UserID = @UserID AND Status = 0)
    BEGIN
        RETURN;
    END
    INSERT INTO SellerApply (UserID, Phone, RealName, IDNumber, ApplyTime, Status)
    VALUES (@UserID, @Phone, @RealName, @IDCard, GETDATE(), 0);
    SELECT SCOPE_IDENTITY();
END;

-- 管理员审核商家申请
DROP PROCEDURE IF EXISTS sp_HandleSellerApply;
CREATE PROCEDURE sp_HandleSellerApply
    @ApplyID INT,
    @Status BIT
AS
BEGIN
    UPDATE SellerApply
    SET Status = @Status
    WHERE ApplyID = @ApplyID;
END;

-- 更新用户信息
DROP PROCEDURE IF EXISTS sp_UpdateUserInfo;
CREATE PROCEDURE sp_UpdateUserInfo
    @UserID INT,
    @UserName VARCHAR(20),
    @Password VARCHAR(20)
AS
BEGIN
    IF EXISTS (SELECT UserID FROM Users WHERE UserName = @UserName AND UserID != @UserID)
    BEGIN
        RETURN;
    END
    UPDATE Users
    SET UserName = @UserName, Password = @Password
    WHERE UserID = @UserID;
    SELECT @UserID;
END;

-- 添加地址
DROP PROCEDURE IF EXISTS sp_AddAddress;
CREATE PROCEDURE sp_AddAddress
    @UserID INT,
    @Receiver VARCHAR(20),
    @Phone VARCHAR(20),
    @Address VARCHAR(100)
AS
BEGIN
    INSERT INTO Address (UserID, Receiver, Phone, Address, IsDefault)
    VALUES (@UserID, @Receiver, @Phone, @Address, 0);
    SELECT SCOPE_IDENTITY();
END;

-- 用户将商品加入购物车
DROP PROCEDURE IF EXISTS sp_AddGoodsToCart;
CREATE PROCEDURE sp_AddGoodsToCart
    @UserID INT,
    @GoodsID INT,
    @Quantity INT
AS
BEGIN
    IF EXISTS (SELECT * FROM CartGoods WHERE UserID = @UserID AND GoodsID = @GoodsID)
    BEGIN
        UPDATE CartGoods
        SET Quantity = Quantity + @Quantity
        WHERE UserID = @UserID AND GoodsID = @GoodsID;
        SELECT UserID FROM CartGoods WHERE UserID = @UserID AND GoodsID = @GoodsID;
    END
    ELSE
    BEGIN
        INSERT INTO CartGoods (UserID, GoodsID, Quantity)
        VALUES (@UserID, @GoodsID, @Quantity);
        SELECT SCOPE_IDENTITY();
    END
END

-- 用户将商品加入收藏
DROP PROCEDURE IF EXISTS sp_AddGoodsToCollect;
CREATE PROCEDURE sp_AddGoodsToCollect
    @UserID INT,
    @GoodsID INT
AS
BEGIN
    IF EXISTS (SELECT CollectID FROM Collect WHERE UserID = @UserID AND GoodsID = @GoodsID)
    BEGIN
        RETURN;
    END
    INSERT INTO Collect (UserID, GoodsID, CreateTime)
    VALUES (@UserID, @GoodsID, GETDATE());
    SELECT SCOPE_IDENTITY();
END;

