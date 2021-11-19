USE BookDB; 
GO

CREATE TABLE Publisher (
   idPublisher int IDENTITY (1,1) PRIMARY KEY,
   PubName varchar(128) NOT NULL,
   PubAddress varchar(500) NOT NULL,
   PubPhone char(11) UNIQUE NOT NULL 
)
GO

CREATE TABLE Author (
   idAuthor int IDENTITY (1,1) PRIMARY KEY,
   AuthorName varchar(128) NOT NULL,
   AuthorBirthDate date NOT NULL,
   AuthorDeathDate date
)
GO

CREATE TABLE Auth (
   idAuth int IDENTITY (1,1) PRIMARY KEY,
   Login varchar(128) UNIQUE NOT NULL ,
   Password varchar(128) NOT NULL
)
GO

CREATE TABLE Status (
    idStatus int IDENTITY (1,1) PRIMARY KEY,
    StatusName varchar(128) NOT NULL,
	StatusDescription varchar(1024) NOT NULL
)
GO

CREATE TABLE Store (
    idStore int IDENTITY (1,1) PRIMARY KEY,
    StoreAddress varchar(500) NOT NULL,
	StorePhone char(11) NOT NULL
)
GO 

CREATE TYPE year 
	FROM decimal(4, 0) NOT NULL

CREATE TABLE Book (
    idBook int IDENTITY (1,1) PRIMARY KEY,
    BookTitle varchar(512) NOT NULL,
    PublishDate year,
	PublisherId int NOT NULL,
	AuthorId int,
	Price smallmoney NOT NULL,
	BookDescription varchar(2000),
	FOREIGN KEY (PublisherId)  REFERENCES Publisher (idPublisher)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
	FOREIGN KEY (AuthorId)  REFERENCES Author (idAuthor)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)
GO

CREATE TABLE Director (
	idDirector int IDENTITY (1,1) PRIMARY KEY,
    AuthId int NOT NULL,
    DirectorName varchar(128) NOT NULL,
	FOREIGN KEY (AuthId)  REFERENCES Auth (idAuth)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)
GO

CREATE TABLE Manager (
	idManager int IDENTITY (1,1) PRIMARY KEY,
    AuthId int NOT NULL,
    ManagerName varchar(128) NOT NULL,
	ManagerPhone char(11) NOT NULL, 
	Salary smallmoney NOT NULL,
	isActive bit,
	FOREIGN KEY (AuthId)  REFERENCES Auth (idAuth)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)
GO

CREATE TABLE Customer (
	idCustomer int IDENTITY (1,1) PRIMARY KEY,
    AuthId int NOT NULL,
    CustomerName varchar(128) NOT NULL,
	CustomerPhone char(11) NOT NULL, 
	Email varchar(128),
	BirthDate date,
	isActive bit,
	FOREIGN KEY (AuthId)  REFERENCES Auth (idAuth)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)
GO

CREATE TABLE BookStore (
	idBookStore int IDENTITY (1,1) PRIMARY KEY,
	BookId int NOT NULL,
	StoreId int NOT NULL,
	Amount int CHECK (Amount >= 0)
	FOREIGN KEY (BookId)  REFERENCES Book (idBook)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
	FOREIGN KEY (StoreId)  REFERENCES Store (idStore)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)

CREATE TABLE [dbo].[Order] (
	idOrder int IDENTITY (1,1) PRIMARY KEY,
	CustomerId int NOT NULL,
	StoreId int NOT NULL,
	OrderDate date NOT NULL,
	OrderPrice money NOT NULL
	FOREIGN KEY (CustomerId)  REFERENCES Customer (idCustomer)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
	FOREIGN KEY (StoreId)  REFERENCES Store (idStore)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)

CREATE TABLE BookOrder (
	idBookOrder int IDENTITY (1,1) PRIMARY KEY,
	OrderId int NOT NULL,
	BookId int NOT NULL,
	Amount int CHECK (Amount > 0)
	FOREIGN KEY (OrderId)  REFERENCES [dbo].[Order] (idOrder)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
	FOREIGN KEY (BookId)  REFERENCES Book (idBook)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)

CREATE TABLE OrderStatus (
	idOrderStatus int IDENTITY (1, 1) PRIMARY KEY,
	OrderId int NOT NULL,
	StatusId int NOT NULL,
	ChangedAt date NOT NULL,
	FOREIGN KEY (OrderId)  REFERENCES [dbo].[Order] (idOrder)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
	FOREIGN KEY (StatusId)  REFERENCES Status (idStatus)
        ON DELETE CASCADE
        ON UPDATE CASCADE
)