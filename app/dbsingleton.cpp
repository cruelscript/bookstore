#include "dbsingleton.h"
#include <QDebug>
#include <QSqlError>

DBSingleton::DBSingleton()
{
  QString host = "COMPUTER-JAKE\\SQLEXPRESS";
  QString database = "BookDB";
  db_ = QSqlDatabase::addDatabase("QODBC");
  db_.setDatabaseName(QString(
        "DRIVER={SQL Server};"
        "Server=%1;"
        "Database=%2;"
        "Persist Security Info=true;"
        "Trusted_Connection=yes").arg(host, database));

  if(!db_.open())
  {
      qDebug() << db_.lastError().text();
  }
  else
  {
      qDebug() << "ok";
  }
}
