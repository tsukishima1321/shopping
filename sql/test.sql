-- 初步测试语句

-- 用户登录
EXEC sp_UserLogin 'user1', '123456';

-- 设置默认地址
EXEC sp_SetDefaultAddress 1, 2;

-- 用户将商品加入购物车
INSERT INTO CartGoods (UserID, GoodsID, Quantity)
VALUES (1, 1, 1),
       (1, 2, 2),
       (1, 4, 3);

-- 用户将购物车提交为订单
EXEC sp_SubmitOrder 1;

-- 商家处理订单
-- 从储存过程中获取订单ID
DECLARE @Table TABLE (OrderID INT);
INSERT INTO @Table
EXEC sp_QueryOrder 2;
SELECT * FROM "OrderDetail" WHERE OrderID IN (SELECT OrderID FROM @Table);
UPDATE Orders
SET Status = 1
WHERE OrderID IN (SELECT OrderID FROM @Table);

-- 用户收藏商品
EXEC sp_AddToCollect 1, 1;

-- 用户查看收藏
SELECT * FROM "GoodsDetail" WHERE GoodsID IN (SELECT GoodsID FROM Collect WHERE UserID = 1);

-- 用户评论商品
EXEC sp_AddComment 1, 1, '商品很好，很喜欢';

-- 用户查看评论
SELECT * FROM CommentDetail WHERE CommentID IN (SELECT CommentID FROM Comment WHERE "GoodsID" = 1);

-- 用户查看订单
SELECT * FROM "OrderDetail" WHERE UserID = 1;

-- 用户查看订单中的商品
SELECT * FROM OrderGoodsDetail WHERE OrderID = 1;

-- 用户查看购物车
SELECT * FROM CartGoodsDetail WHERE UserID = 1;

-- 用户将商品加入购物车
EXEC sp_AddToCart 1, 3, 1;
EXEC sp_AddToCart 1, 5, 2;

-- 商家开店
EXEC sp_AddShop 2, '商店2', '商店2描述';
-- 返回了商店ID

-- 商家添加商品
EXEC sp_AddGoods 4, '商品2', '商品2描述', 100;
EXEC sp_AddGoods 4, '商品3', '商品3描述', 200;
-- 返回了商品ID
UPDATE Goods SET Image = 'image2' WHERE GoodsID = 11;

-- 激活店铺
EXEC sp_ActivateShop 4;

-- 下架商品
EXEC sp_DeactiveGoods 3;
