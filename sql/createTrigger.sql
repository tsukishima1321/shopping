-- 创建触发器

-- 用户注册时，自动添加用户权限
DROP TRIGGER IF EXISTS tr_UserCreate;
CREATE TRIGGER tr_UserCreate ON Users FOR INSERT
AS
-- Usu curser to iterate through the inserted table
DECLARE insert_cursor CURSOR LOCAL FOR SELECT UserID FROM inserted
DECLARE @UserID INT
OPEN insert_cursor
FETCH NEXT FROM insert_cursor INTO @UserID
WHILE @@FETCH_STATUS = 0
BEGIN
    INSERT INTO UserPermission (UserID, AllowLogin, AllowShopping, AllowComment, AllowAddShop, AllowAddGoods, AllowHandleOrder)
    VALUES (@UserID, 1, 1, 1, 0, 0, 0);
    FETCH NEXT FROM insert_cursor INTO @UserID
END;

-- 用户认证成为商家时，自动更新用户权限
DROP TRIGGER IF EXISTS tr_UserAuthSeller;
CREATE TRIGGER tr_UserAuthSeller ON Seller FOR INSERT
AS
DECLARE insert_cursor CURSOR LOCAL FOR SELECT UserID FROM inserted
DECLARE @UserID INT
OPEN insert_cursor
FETCH NEXT FROM insert_cursor INTO @UserID
WHILE @@FETCH_STATUS = 0
BEGIN
    UPDATE UserPermission
    SET AllowAddShop = 1, AllowAddGoods = 1, AllowHandleOrder = 1
    WHERE UserID = @UserID;
    FETCH NEXT FROM insert_cursor INTO @UserID
END;

-- 插入地址时，若只有一个地址，则自动设置为默认地址
DROP TRIGGER IF EXISTS tr_AddressCreate;
CREATE TRIGGER tr_AddressCreate ON Address AFTER INSERT
AS
DECLARE @UserID INT
SELECT @UserID = UserID FROM inserted
IF (SELECT COUNT(*) FROM Address WHERE UserID = @UserID) = 1
BEGIN
    UPDATE Address
    SET IsDefault = 1
    WHERE UserID = @UserID
END;

-- 删除地址时，若删除的是默认地址，则自动设置一个地址为默认地址
DROP TRIGGER IF EXISTS tr_AddressDelete;
CREATE TRIGGER tr_AddressDelete ON Address AFTER DELETE
AS
DECLARE @UserID INT
SELECT @UserID = UserID FROM deleted
IF (SELECT COUNT(*) FROM Address WHERE UserID = @UserID) > 0
BEGIN
    IF NOT EXISTS (SELECT * FROM Address WHERE UserID = @UserID AND IsDefault = 1)
    BEGIN
        UPDATE Address
        SET IsDefault = 1
        WHERE UserID = @UserID
        AND AddressID = (SELECT TOP 1 AddressID FROM Address WHERE UserID = @UserID)
    END
END;

-- 删除地址前，若有未处理的订单，则禁止删除
DROP TRIGGER IF EXISTS tr_AddressDeleteCheck;
CREATE TRIGGER tr_AddressDeleteCheck ON Address FOR DELETE
AS
DECLARE @AddressID INT
DECLARE @UserID INT
SELECT @AddressID = AddressID, @UserID = UserID FROM deleted
IF EXISTS (SELECT * FROM Orders WHERE AddressID = @AddressID AND Status = 0)
BEGIN
    RAISERROR('There are unhandled orders associated with this address, please handle them first.', 16, 1)
    ROLLBACK TRANSACTION
END;

-- 认证申请被接受时，自动更新商家信息
DROP TRIGGER IF EXISTS tr_AuthAccept;
CREATE TRIGGER tr_AuthAccept ON SellerApply FOR UPDATE
AS
BEGIN
    DECLARE @UserID INT
    DECLARE @RealName VARCHAR(20)
    DECLARE @IDNumber VARCHAR(20)
    DECLARE @Phone VARCHAR(20)
    DECLARE @Result INT
    SELECT @UserID = UserID, @RealName = RealName, @IDNumber = IDNumber, @Phone = Phone, @Result = Status FROM inserted;
    IF @Result = 1
    BEGIN
        IF EXISTS (SELECT * FROM Seller WHERE UserID = @UserID)
        BEGIN
            UPDATE Seller
            SET RealName = @RealName, IDNumber = @IDNumber, Phone = @Phone
            WHERE UserID = @UserID
            UPDATE Users SET IsSeller = 1 WHERE UserID = @UserID
        END
        ELSE
        BEGIN
            INSERT INTO Seller (UserID, RealName, IDNumber, Phone)
            VALUES (@UserID, @RealName, @IDNumber, @Phone)
            UPDATE Users SET IsSeller = 1 WHERE UserID = @UserID
        END
    END
END;

-- 店铺被关闭时，自动下架所有商品
DROP TRIGGER IF EXISTS tr_ShopClose;
CREATE TRIGGER tr_ShopClose ON Shop FOR UPDATE
AS
BEGIN
    DECLARE @ShopID INT
    DECLARE @Status INT
    SELECT @ShopID = ShopID, @Status = Status FROM inserted
    IF @Status = 1
    BEGIN
        UPDATE Goods
        SET Status = 1
        WHERE ShopID = @ShopID
    END
END;