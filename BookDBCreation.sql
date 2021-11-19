USE master
GO

CREATE DATABASE BookDB ON (
    Name= BookDB, 
    FileName='C:\Program Files\Microsoft SQL Server\MSSQL15.SQLEXPRESS\MSSQL\DATA\BookDB.mdf',
    Maxsize=100
) 
LOG ON (
    Name= BookDB_log, 
    FileName='C:\Program Files\Microsoft SQL Server\MSSQL15.SQLEXPRESS\MSSQL\DATA\BookDB_log.ldf',
    Maxsize=100
)
GO

EXEC SP_HELPDB BookDB
GO
ALTER DATABASE BookDB SET AUTO_SHRINK ON 
GO