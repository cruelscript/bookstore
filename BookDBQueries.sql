USE BookDB
GO

CREATE MASTER KEY ENCRYPTION BY
    PASSWORD = 'superduperpassword#777'

CREATE CERTIFICATE CERTIFICATE
   WITH SUBJECT = 'Certificate for Admin DB'

CREATE SYMMETRIC KEY SSN_Key_01
    WITH ALGORITHM = AES_256
    ENCRYPTION BY CERTIFICATE CERTIFICATE
GO

SET XACT_ABORT ON
GO

OPEN SYMMETRIC KEY SSN_Key_01
	DECRYPTION BY CERTIFICATE CERTIFICATE
GO

-- 1.2. Добавление нового склада
CREATE OR ALTER PROCEDURE AddStore (
	@Address AS varchar(500),
	@Phone AS char(15)
) AS 
	INSERT INTO Store VALUES (
		@Address,
		@Phone
	)
GO

EXEC AddStore 'SPB, Nevsky Ave, 1', '9876532113'
GO

-- 1.3. Добавление новой книги
CREATE OR ALTER PROCEDURE AddBook (
	@Title AS varchar(512),
	@PublishYear AS year,
	@PublisherId AS int,
	@AuthorId AS int,
	@Price AS smallmoney,
	@Description AS varchar(2000)
) AS
	INSERT INTO Book VALUES (
		@Title,
		@PublishYear,
		@PublisherId,
		@AuthorId,
		@Price,
		@Description
	)
	DECLARE @id AS int
	SET @id = SCOPE_IDENTITY()
	SELECT @id AS id
GO

-- 1.4. Добавление нового издательства
CREATE OR ALTER PROCEDURE AddPublisher (
	@Name AS varchar(128),
	@Address AS varchar(500),
	@Phone AS char(15)
) AS 
	INSERT INTO Publisher VALUES (
		@Name,
		@Address,
		@Phone
	)
GO

AddPublisher 'new', 'klkjsadakjsd', '18219833'
SELECT * FROM Publisher

-- 1.5. Добавление нового автора
CREATE OR ALTER PROCEDURE AddAuthor (
	@Name AS varchar(128),
	@BirthDate AS date,
	@DeathDate AS date
) AS 
	INSERT INTO Author VALUES (
		@Name,
		@BirthDate,
		@DeathDate
	)
GO

SELECT * FROM Author
AddAuthor keke, '1967-08-09', NULL
SELECT * FROM Book WHERE AuthorId = 20
RemoveAuthor 20

-- 1.6. Добавление нового заказа

CREATE TYPE OrderBook AS TABLE (
	[BookId] INT NOT NULL,
	[Amount] INT NOT NULL
)
GO

CREATE OR ALTER PROCEDURE AddOrder (
	@CustomerId AS int,
	@StoreId AS int,
	@OrderDate AS date,
	@OrderPrice AS money
) AS 
	INSERT INTO [dbo].[Order] VALUES (
		@CustomerId,
		@StoreId,
		@OrderDate,
		@OrderPrice
	)
	DECLARE @id AS int
	SET @id = SCOPE_IDENTITY()

	INSERT INTO OrderStatus VALUES (
		@id, 
		1, 
		(SELECT CAST( GETDATE() AS Date))
	)
	SELECT @id AS id
GO

CREATE OR ALTER PROCEDURE AddBookOrder (
	@CustomerId AS int,
	@StoreId AS int,
	@OrderDate AS date,
	@Books AS OrderBook READONLY
) AS 
	DECLARE @OrderPrice AS money
	DECLARE @OrderId AS int
	SET @OrderPrice = (SELECT SUM(Price) FROM @Books JOIN Book ON 
							BookId = idBook)
	EXEC AddOrder @CustomerId, @StoreId, @OrderDate, @OrderPrice, @OrderId OUTPUT
	INSERT INTO BookOrder SELECT @OrderId, t.* FROM (
		SELECT * FROM @Books
	) AS t
GO

DECLARE @BookTable AS OrderBook
INSERT INTO @BookTable VALUES 
	(19, 1),
	(26, 1)

EXEC AddBookOrder 1, 2, '2021-12-06', @BookTable 
GO

SELECT * FROM [dbo].[Order]
SELECT * FROM BookOrder
GO

-- 1.7. Изменение количества книг на конкретном складе
CREATE OR ALTER PROCEDURE UpdateBookStore (
	@BookId AS int, 
	@StoreId AS int, 
	@Amount AS int
) AS 
	UPDATE BookStore SET Amount = @Amount
		WHERE BookId = @BookId AND StoreId = @StoreId
GO

EXEC UpdateBookStore 37, 1, 17
GO

-- 1.8. Изменение статуса заказа в БД
CREATE OR ALTER PROCEDURE UpdateOrderStatus (
	@OrderId AS int, 
	@StatusId AS int
) AS
	UPDATE OrderStatus SET StatusId = @StatusId, ChangedAt = CAST(GETDATE() AS date)
		WHERE OrderId = @OrderId
GO

-- 1.9. Изменение информации о книге
CREATE OR ALTER PROCEDURE UpdateBook (
	@id AS int, 
	@Title AS varchar(512),
	@PublishYear AS year,
	@PublisherId AS int,
	@AuthorId AS int,
	@Price AS smallmoney,
	@Description AS varchar(2000)
) AS 
	UPDATE Book SET BookTitle = @Title 
		WHERE idBook = @id AND @Title != BookTitle
	UPDATE Book SET PublishYear = @PublishYear 
		WHERE idBook = @id AND @PublishYear != PublishYear
	UPDATE Book SET PublisherId = @PublisherId 
		WHERE idBook = @id AND @PublisherId != PublisherId
	UPDATE Book SET AuthorId = @AuthorId 
		WHERE idBook = @id AND @AuthorId != AuthorId
	UPDATE Book SET Price = @Price 
		WHERE idBook = @id AND @Price != Price
	UPDATE Book SET BookDescription = @Description 
		WHERE idBook = @id AND @Description != BookDescription
GO

-- 1.10. Изменение информации об авторе
CREATE OR ALTER PROCEDURE UpdateAuthor (
	@id AS int,
	@Name AS varchar(128),
	@BirthDate AS date,
	@DeathDate AS date
) AS 
	UPDATE Author SET AuthorName = @Name
		WHERE idAuthor = @id AND @Name != AuthorName
	UPDATE Author SET AuthorBirthDate = @BirthDate
		WHERE idAuthor = @id AND @BirthDate != AuthorBirthDate
	UPDATE Author SET AuthorDeathDate = @DeathDate
		WHERE idAuthor = @id AND @DeathDate != AuthorDeathDate
GO

-- 1.11. Изменение информации об издательстве
CREATE OR ALTER PROCEDURE UpdatePublisher (
	@id AS int, 
	@Name AS varchar(128),
	@Address AS varchar(500),
	@Phone AS char(15)
) AS
	UPDATE Publisher SET PubName = @Name 
		WHERE idPublisher = @id AND @Name != PubName
	UPDATE Publisher SET PubAddress = @Address 
		WHERE idPublisher = @id AND @Address != PubAddress
	UPDATE Publisher SET PubPhone = @Phone 
		WHERE idPublisher = @id AND @Phone != PubPhone
GO

-- 1.12. Изменение информации о складе
CREATE OR ALTER PROCEDURE UpdateStore (
	@id AS int, 
	@Address AS varchar(500),
	@Phone AS char(15)
) AS
	UPDATE Store SET StoreAddress = @Address
		WHERE idStore = @id AND @Address != StoreAddress
	UPDATE Store SET StorePhone = @Phone
		WHERE idStore = @id AND @Phone != StorePhone
GO

-- 1.13. Изменение личных данных пользователя
CREATE OR ALTER PROCEDURE UpdateCustomer (
	@id AS int,
	@Name AS varchar(128),
	@Phone AS char(15),
	@Email AS varchar(128),
	@BirthDate AS date
) AS
	UPDATE Customer SET CustomerName = @Name
		WHERE idCustomer = @id AND @Name != CustomerName
	UPDATE Customer SET CustomerPhone = @Phone
		WHERE idCustomer = @id AND @Phone != CustomerPhone
	UPDATE Customer SET Email = @Email
		WHERE idCustomer = @id AND @Email != Email
	UPDATE Customer SET BirthDate = @BirthDate
		WHERE idCustomer = @id AND @BirthDate != BirthDate
GO

-- 1.15. Изменение личных данных сотрудника
CREATE OR ALTER PROCEDURE UpdateManager (
	@id AS int,
	@Name AS varchar(128),
	@Phone AS char(15)
) AS 
	UPDATE Manager SET ManagerName = @Name
		WHERE idManager = @id AND @Name != ManagerName
	UPDATE Manager SET ManagerPhone = @Phone
		WHERE idManager = @id AND @Phone != ManagerPhone
GO

-- 1.17. Изменение цены на книгу
CREATE OR ALTER PROCEDURE UpdateBookPrice (
	@id AS int, 
	@Price AS smallmoney
) AS 
	UPDATE Book SET Price = @Price
		WHERE idBook = @id AND @Price != Price
GO

EXEC UpdateBookPrice 19, 10000
GO

CREATE OR ALTER PROCEDURE UpdateAmount(@amount as int, @bookId as int, @storeId as int) As
	UPDATE BookStore SET Amount = Amount - @amount WHERE BookId = @bookId AND StoreId = @storeId

UpdateAmount 1, 23, 1
SELECT * FROM BookStore
SELECT * FROM BookOrder
GO

select * from auth

-- 1.18. Изменение заработной платы сотрудников
CREATE OR ALTER PROCEDURE UpdateManagerSalary (
	@id AS int, 
	@Salary AS smallmoney
) AS 
	UPDATE Manager SET Salary = @Salary
		WHERE idManager = @id AND @Salary != Salary
GO

EXEC UpdateManagerSalary 1, 23000
GO

-- 1.19. Вывод данных о сотрудниках
CREATE OR ALTER VIEW Managers AS
	SELECT idManager AS Identificator, ManagerName, ManagerPhone, Salary 
		FROM Manager JOIN Auth
			ON AuthId = idAuth
			WHERE isActive = 1
GO

SELECT * FROM Managers
GO

CREATE OR ALTER PROCEDURE GetInfo (@id AS int) AS
	SELECT * FROM Manager 
		WHERE idManager = @id
GO

-- 2.1. Просмотр продаж какой-то конкретной книги за период
CREATE OR ALTER PROCEDURE GetSales (
	@start AS date, 
	@end AS date,
	@id AS int
) AS
	SELECT BookTitle, CustomerName, OrderDate, Amount FROM [dbo].[Order] 
		JOIN Customer ON CustomerId = idCustomer
		JOIN BookOrder ON OrderId = idOrder
		JOIN Book ON idBook = BookId
			WHERE OrderDate >= @start 
				AND OrderDate <= @end
				AND BookId = @id
GO

EXEC GetSales '2021-12-04', '2021-12-05', 31
GO

-- 2.2. Просмотр книг, которые необходимо доставить на склад
CREATE OR ALTER PROCEDURE NeededBooks (@id AS int) AS
	SELECT BookTitle, SUM(t.Amount) AS Amount FROM 
		((SELECT BookId, Amount FROM BookOrder JOIN [dbo].[Order]
			ON idOrder = OrderId
				WHERE StoreId = @id) 
		EXCEPT
		(SELECT BookId, Amount FROM BookStore
			WHERE StoreId = @id)) AS t
		JOIN Book ON t.BookId = idBook
		JOIN BookOrder ON t.BookId = BookOrder.BookId
		JOIN [dbo].[Order] ON OrderId = [dbo].[Order].idOrder 
		JOIN OrderStatus ON idOrder = OrderStatus.OrderId
			WHERE [dbo].[Order].StoreId = @id 
				AND StatusId = 1 
				GROUP BY BookTitle
GO

EXEC NeededBooks 3
GO



-- 2.3. Вывод списка книг, удовлетворяющих входным данным
CREATE OR ALTER PROCEDURE SearchByAuthor (@Author AS varchar(128)) AS
	SELECT BookTitle, AuthorName, PublishYear, PubName, Price, BookDescription FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			WHERE AuthorName = @Author
GO

EXEC SearchByAuthor 'Герман Гессе'
GO

CREATE OR ALTER PROCEDURE SearchByTitle (@Title AS varchar(128)) AS
	SELECT BookTitle, AuthorName, PublishYear, PubName, Price, BookDescription FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			WHERE BookTitle = @Title
GO

CREATE OR ALTER PROCEDURE SearchByWord (@Word AS varchar(128)) AS
	SELECT DISTINCT idBook, BookTitle AS 'Название', AuthorName AS 'Автор' FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
		JOIN BookStore ON BookId = idBook
			WHERE (CHARINDEX(@Word, BookTitle) != 0 
				OR CHARINDEX(@Word, AuthorName) != 0 
				OR CHARINDEX(@Word, PubName) != 0
				OR CHARINDEX(@Word, BookDescription) != 0)
				AND Amount > 0
GO

EXEC SearchByWord 'Мастер'
GO

-- 2.4. Просмотр подробной информации о книге
CREATE OR ALTER PROCEDURE BookInfo (@id AS int) AS
	SELECT BookTitle AS BT, AuthorName AS AN, PublishYear AS PY, PubName AS PN, Price AS P, BookDescription AS BD FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			WHERE idBook = @id
GO

EXEC BookInfo 17
GO

-- 2.5. Просмотр каталога книг
CREATE OR ALTER PROCEDURE BookList AS
SELECT DISTINCT BookTitle, AuthorName, PublishYear, PubName, Price, BookDescription FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
		JOIN BookStore ON idBook = BookId
			WHERE Amount > 0
GO

-- 2.6. Просмотр всех книг издательства
CREATE OR ALTER PROCEDURE PublisherBooks (@id AS int) AS
	SELECT BookTitle, AuthorName, PublishYear, Price, BookDescription FROM Book 
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			WHERE idPublisher = @id
GO

EXEC PublisherBooks 4
GO

-- 2.7. Вывод всех книг, которые есть на конкретном складе
CREATE OR ALTER PROCEDURE StoreBooks (@id AS int) AS 
	SELECT idBook, BookTitle AS 'Название', AuthorName AS 'Автор', Amount AS 'Кол-во' FROM Book
		JOIN Author ON idAuthor = AuthorId
		JOIN BookStore ON idBook = BookId
			WHERE StoreId = @id 
GO

EXEC StoreBooks 2
GO

SELECT * FROM Auth

CREATE OR ALTER PROCEDURE OrderStore(@id AS int) AS 
	SELECT idOrder, CustomerName AS 'ФИО Покупателя', StatusName AS 'Статус заказа', ChangedAt AS 'Последнее изменение', OrderDate AS 'Дата заказа', OrderPrice AS 'Цена' FROM [dbo].[Order]
		JOIN Store ON idStore = StoreId
		JOIN OrderStatus ON idOrder = OrderId
		JOIN [dbo].[Status] ON idStatus = StatusId
		JOIN Customer ON idCustomer = CustomerId
		WHERE idStore = @id
GO
OrderStore 3
GO

-- 2.8. Просмотр книг в заказе
CREATE OR ALTER PROCEDURE OrderBooks (@id AS int) AS
	SELECT BookTitle AS 'Название', AuthorName AS 'Автор', Amount AS 'Кол-во' FROM [dbo].[Order]
		JOIN BookOrder ON OrderId = idOrder
		JOIN Book ON BookId = idBook
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			WHERE idOrder = @id
GO

EXEC OrderBooks 9
GO

-- 3.1. Вывод всех книг по убыванию их популярности
CREATE OR ALTER VIEW BookPopularity AS
	SELECT TOP(100) idBook, BookTitle AS 'Название', AuthorName AS 'Автор' FROM Book 
		LEFT JOIN BookOrder ON idBook = BookId
		JOIN Author ON idAuthor = AuthorId
		JOIN Publisher ON idPublisher = PublisherId
			GROUP BY idBook, BookTitle, AuthorName, PublishYear, PubName, Price, BookDescription
				ORDER BY SUM(Amount) DESC
GO

SELECT * FROM BookPopularity
GO

-- 3.2. Вывод списка всех заказов, совершенных пользователем, отсортированных по времени создания.
CREATE OR ALTER VIEW CustomersOrders AS
	SELECT TOP(100) CustomerName, StatusName, ChangedAt, OrderDate, OrderPrice, StoreAddress FROM [dbo].[Order]
		JOIN Store ON idStore = StoreId
		JOIN OrderStatus ON idOrder = OrderId
		JOIN [dbo].[Status] ON idStatus = StatusId 
		JOIN Customer ON CustomerId = idCustomer
		JOIN Auth ON AuthId = idAuth
		WHERE isActive = 1 
			GROUP BY CustomerName, StatusName, ChangedAt, OrderDate, OrderPrice, StoreAddress
				ORDER BY OrderDate DESC
GO 

SELECT * FROM CustomersOrders
GO

CREATE OR ALTER PROCEDURE CustomerOrders (@id AS int) AS
	SELECT idOrder, StatusName AS 'Статус заказа', ChangedAt AS 'Последнее изменение', OrderDate AS 'Дата заказа', OrderPrice AS 'Цена', StoreAddress AS 'Адрес склада', StorePhone AS 'Телефон склада' FROM [dbo].[Order]
		JOIN Store ON idStore = StoreId
		JOIN OrderStatus ON idOrder = OrderId
		JOIN [dbo].[Status] ON idStatus = StatusId 
		WHERE CustomerId = @id
			GROUP BY idOrder, StatusName, ChangedAt, OrderDate, OrderPrice, StoreAddress, StorePhone
				ORDER BY OrderDate DESC
GO

EXEC CustomerOrders 21
GO

SELECT idOrder, BookTitle AS 'Название', AuthorName AS 'Автор', Price AS 'Цена', Amount AS 'Кол-во' FROM Book 
                                     JOIN Author ON idAuthor = authorId
									 JOIN BookOrder ON idBook = BookId
                                     JOIN [dbo].[Order] ON idOrder = OrderId
										WHERE idOrder = 23 AND CustomerId = 21

SELECT * FROM OrderStatus

-- 1.14. Изменение авторизационных данных пользователя (логин/пароль)
CREATE OR ALTER PROCEDURE AddAuth (
	@Login AS varchar(128),
	@Password AS varchar(128),
	@idAuth AS int OUTPUT
) AS 
	BEGIN TRANSACTION 
		OPEN SYMMETRIC KEY SSN_Key_01
		DECRYPTION BY CERTIFICATE CERTIFICATE
		INSERT INTO Auth VALUES (
			@Login, 
			ENCRYPTBYKEY(KEY_GUID('SSN_KEY_01'), @Password),
			1
		)
		SET @idAuth = SCOPE_IDENTITY()
	COMMIT TRANSACTION
GO

DECLARE @output AS int
EXEC AddAuth tryk1, try10, @output OUTPUT
SELECT * FROM Auth
GO


CREATE OR ALTER PROCEDURE UpdateAuth (
	@id AS int, 
	@Login AS varchar(128),
	@Password AS varchar(128)
) AS 
	BEGIN TRANSACTION 
		OPEN SYMMETRIC KEY SSN_Key_01
		DECRYPTION BY CERTIFICATE CERTIFICATE
		UPDATE Auth SET Auth.[Login] = @Login
			WHERE idAuth = @id AND @Login != Auth.[Login]
		UPDATE Auth SET Auth.[Password] = ENCRYPTBYKEY(KEY_GUID('SSN_KEY_01'), @Password)
			WHERE idAuth = @id --AND --@Password != CONVERT(char, DECRYPTBYKEY(Auth.[Password]))
	COMMIT TRANSACTION
GO

EXEC UpdateAuth 3, Pozetiv4ik, NePlach1234
SELECT * FROM Auth
GO

CREATE OR ALTER PROCEDURE GetAuthId (
	@Login AS varchar(128),
	@Password AS varchar(128),
	@AuthId AS int OUTPUT
) AS
	SET @AuthId = (SELECT idAuth FROM Auth
						WHERE Auth.[Login] = @Login
							AND CONVERT(char, DECRYPTBYKEY(Auth.[Password])) = @Password)
GO

CREATE OR ALTER PROCEDURE Authorize (
	@Login as varchar(128),
	@Password as varchar(128)
) AS
	BEGIN TRANSACTION
		OPEN SYMMETRIC KEY SSN_Key_01
		DECRYPTION BY CERTIFICATE CERTIFICATE
		DECLARE @AuthId AS int
		EXEC GetAuthId @Login, @Password, @AuthId OUTPUT
		IF (@AuthId IS NULL) 
			BEGIN
				RAISERROR('User Not Found', 18, 1)
				RETURN
			END
		(SELECT idDirector AS id, 'Director' AS job FROM Director
			WHERE Director.AuthId = @AuthId)
		UNION
		(SELECT idManager AS id, 'Manager' AS job FROM Manager
			WHERE Manager.AuthId = @AuthId)
		UNION
		(SELECT idCustomer AS id, 'Customer' AS job FROM Customer
			WHERE Customer.AuthId = @AuthId)
	COMMIT TRANSACTION
GO

SELECT * FROM Auth
GO

-- 1.1. Деактивация профиля пользователя
CREATE OR ALTER PROCEDURE RemoveManager (
	@ManagerId AS int
) AS 
	BEGIN TRANSACTION
		DECLARE @AuthId AS int
		SET @AuthId = (SELECT AuthId FROM Manager WHERE idManager = @ManagerId)
		UPDATE Auth SET isActive = 0 
			WHERE idAuth = @AuthId
		DELETE FROM Manager WHERE idManager = @ManagerId
	COMMIT TRANSACTION
GO

-- 4.1. Регистрация нового сотрудника
CREATE OR ALTER PROCEDURE AddManager (
	@Name AS varchar(128),
	@Phone AS char(15),
	@Salary AS smallmoney,
	@Login AS varchar(128),
	@Password AS varchar(128)
) AS 
	SET XACT_ABORT ON
	BEGIN TRANSACTION 
		DECLARE @AuthId AS int
		EXEC AddAuth @Login, @Password, @AuthId OUTPUT
		INSERT INTO Manager VALUES (
			@AuthId,
			@Name,
			@Phone,
			@Salary
		)
	COMMIT TRANSACTION
GO

EXEC AddManager 'Mnager', '9875643212', '10000', 'asdfd', '1234567'
GO
EXEC AddManager 'Mnager1', '9875643212', '10000', 'asdfd1', '1234567'
GO
SELECT * FROM AUTH
GO

-- 4.2. Регистрация нового пользователя
CREATE OR ALTER PROCEDURE AddCustomer (
	@Name AS varchar(128),
	@Phone AS char(15),
	@Email AS varchar(128),
	@BirthDate AS date, 
	@Login AS varchar(128),
	@Password AS varchar(128)
) AS
	SET XACT_ABORT ON
	BEGIN TRANSACTION
		DECLARE @AuthId AS int
		EXEC AddAuth @Login, @Password, @AuthId OUTPUT
		INSERT INTO Customer VALUES (
			@AuthId,
			@Name,
			@Phone,
			@Email,
			@BirthDate
		)
	COMMIT TRANSACTION
GO

EXEC AddCustomer 'Vasya Pupkin', '9501851021', 'keklol@gmail.com', '1999-02-02', 'Ded_L0goped', '124qwet'
GO

EXEC AddCustomer 'Vasya Pupkin1', '9501851021', 'keklol@gmail.com', '1999-02-02', 'Ded_L0goped34', '124qwet'
GO


EXEC AddCustomer 'Vasya Pupkin2', '9501851022', 'keklol1@gmail.com', '1999-02-02', 'try', 'try10'
GO


SELECT * FROM Customer
SELECT CONVERT(char,DECRYPTBYKEY([Password])) FROM Auth
GO

-- 1.16. Удаление склада
 CREATE OR ALTER PROCEDURE RemovePublisher(@id AS int) AS
	DELETE FROM Publisher WHERE @id = idPublisher
GO 

EXEC RemovePublisher 4
GO

SELECT * FROM Publisher
GO

CREATE OR ALTER PROCEDURE RemoveStore(@id AS int) AS
	DELETE FROM Store WHERE @id = idStore
GO 

CREATE OR ALTER PROCEDURE RemoveBook(@id AS int) AS
		IF NOT EXISTS (
				SELECT * FROM BookStore
					WHERE BookId = @id AND Amount > 0
			)
		BEGIN
			DELETE FROM Book WHERE @id = idBook
		END
	ELSE RAISERROR('You cannot delete Book that is being used in other tables. ^^', 18, 1)
GO 

 CREATE OR ALTER PROCEDURE RemoveAuthor(@id AS int) AS
	DELETE FROM Author WHERE @id = idAuthor
GO 

-- 5.1. Триггер на удаление из БД записи о книги, которая участвует в заказах

-- 5.2. Триггер на удаление из БД записи об авторе, книги которого присутствуют в заказах
CREATE OR ALTER TRIGGER OnAuthorRemoval ON Author
	INSTEAD OF DELETE AS
		BEGIN
			DECLARE @AuthorId int
			SELECT @AuthorId = deleted.idAuthor FROM deleted
			IF NOT EXISTS (
				SELECT * FROM Book
					WHERE AuthorId = @AuthorId
			) 
				BEGIN
					DELETE FROM Author WHERE idAuthor = @AuthorId
				END
			ELSE RAISERROR('You cannot delete Author that is being used in other tables. ^^', 18, 1)
		END
GO

-- 5.3. Триггер на удаление из БД издательства, книги которого присутствуют в заказах
CREATE OR ALTER TRIGGER OnPublisherRemoval ON Publisher
	INSTEAD OF DELETE AS
		BEGIN
			DECLARE @PublisherId int
			SELECT @PublisherId = deleted.idPublisher FROM deleted
			IF NOT EXISTS (
				SELECT * FROM Book
					WHERE PublisherId = @PublisherId
			) 
				BEGIN
					DELETE FROM Publisher WHERE idPublisher = @PublisherId
				END
			ELSE RAISERROR('You cannot delete Publisher that is being used in other tables. ^^', 18, 1)
		END
GO

-- 5.4. Триггер на удаление из БД склада, на котором остались книги
CREATE OR ALTER TRIGGER OnStoreRemoval ON Store
	INSTEAD OF DELETE AS
		BEGIN
			DECLARE @StoreId int
			SELECT @StoreId = deleted.idStore FROM deleted
			IF NOT EXISTS (
				SELECT StoreId FROM [dbo].[Order]
					WHERE StoreId = @StoreId
				UNION 
				SELECT StoreId FROM BookStore
					WHERE StoreId = @StoreId
			) 
				BEGIN
					DELETE FROM Store WHERE idStore = @StoreId
				END
			ELSE RAISERROR('You cannot delete Store that is being used in other tables. ^^', 18, 1)
		END
GO

ALTER table Store
add constraint AK_Phone UNIQUE (StorePhone)