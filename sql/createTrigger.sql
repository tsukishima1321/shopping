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

