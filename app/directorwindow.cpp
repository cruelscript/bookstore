#include "dbsingleton.h"
#include "directorwindow.h"
#include "ui_directorwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

DirectorWindow::DirectorWindow(QWidget *parent) :
  QDialog(parent),
  bookModel_(new QSqlQueryModel()),
  storeModel_(new QSqlQueryModel()),
  managerModel_(new QSqlQueryModel()),
  ui(new Ui::DirectorWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();

  bookModel_->setQuery(QString("SELECT DISTINCT idBook, BookTitle AS 'Название', AuthorName AS 'Автор' FROM Book "
                          "JOIN Author ON idAuthor = AuthorId "));
  ui->bookTableView->setModel(bookModel_);
  ui->bookTableView->resizeColumnsToContents();
  ui->bookTableView->setColumnHidden(0, true);

  storeModel_->setQuery(QString("SELECT DISTINCT idStore, storeAddress AS 'Адрес', StorePhone AS 'Телефон' FROM  Store"));
  ui->storeTableView->setModel(storeModel_);
  ui->storeTableView->resizeColumnsToContents();
  ui->storeTableView->setColumnHidden(0, true);

  managerModel_->setQuery(QString("SELECT DISTINCT idManager, AuthId, managerName AS 'ФИО', ManagerPhone AS 'Телефон', salary AS 'Зарплата' FROM  Manager"));
  ui->managerTableView->setModel(managerModel_);
  ui->managerTableView->resizeColumnsToContents();
  ui->managerTableView->setColumnHidden(0, true);
  ui->managerTableView->setColumnHidden(1, true);
}


DirectorWindow::~DirectorWindow()
{
  delete ui;
  delete bookModel_;
  delete storeModel_;
  delete managerModel_;
}


void DirectorWindow::refresh()
{
   storeModel_->setQuery(storeModel_->query().lastQuery());
   bookModel_->setQuery(bookModel_->query().lastQuery());
   managerModel_->setQuery(managerModel_->query().lastQuery());
   ui->managerTableView->resizeColumnsToContents();
   ui->storeTableView->resizeColumnsToContents();
}


void DirectorWindow::on_LogOutButton_clicked()
{
    this->close();
    emit firstWindow();
}


void DirectorWindow::on_bookTableView_clicked(const QModelIndex &index)
{
  currentIndex = index;
  QSqlQueryModel* model = new QSqlQueryModel();
  model->setQuery(QString("SELECT CustomerName AS 'ФИО', OrderDate AS 'Дата', Amount AS 'Количество' FROM [dbo].[Order] "
                             "JOIN Customer ON CustomerId = idCustomer "
                             "JOIN BookOrder ON OrderId = idOrder "
                             "JOIN Book ON idBook = BookId "
                                   "WHERE BookId = %1").arg(bookModel_->index(index.row(), 0).data().toInt()));
  ui->salesTableView->setModel(model);
  ui->salesTableView->resizeColumnsToContents();
}


void DirectorWindow::on_bookTableView_doubleClicked(const QModelIndex &index)
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL BookInfo(:id)}");
  query.bindValue(":id", bookModel_->index(index.row(), 0).data().toInt());
  query.exec();
  query.next();

  QSqlRecord record = query.record();

  int title = record.indexOf("BT");
  int author = record.indexOf("AN");
  int year = record.indexOf("PY");
  int publisher = record.indexOf("PN");
  int price = record.indexOf("P");
  int description = record.indexOf("BD");

  bookInformation = new BookInformation(
        record.value(title).toString(),
        record.value(author).toString(),
        record.value(publisher).toString(),
        record.value(year).toString(),
        QString(record.value(price).toString() + " руб"),
        record.value(description).toString());
  bookInformation->show();
}


void DirectorWindow::on_changePriceButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL BookInfo(:id)}");
  query.bindValue(":id", bookModel_->index(currentIndex.row(), 0).data().toInt());
  query.exec();
  query.next();

  QSqlRecord record = query.record();

  int title = record.indexOf("BT");
  int price = record.indexOf("P");


  QInputDialog input;
  QString newPrice = QInputDialog::getText(this,
                                           "Change Price",
                                           record.value(title).toString(),
                                           QLineEdit::Normal,
                                           record.value(price).toString());

  query.prepare("{ CALL UpdateBookPrice(:id, :price)}");
  query.bindValue(":id", bookModel_->index(currentIndex.row(), 0).data().toInt());
  query.bindValue(":price", newPrice.toInt());
  if (newPrice.toInt() > 0)
  {
      query.exec();
  }
}

void DirectorWindow::on_removeBookButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL RemoveBook(:id)}");
  query.bindValue(":id", bookModel_->index(currentIndex.row(), 0).data().toInt());

  if(!query.exec())
  {
      QMessageBox msgBox;
      msgBox.setText("Нельзя удалить книгу. Она есть на складе");
      msgBox.exec();
  }
  refresh();
}


void DirectorWindow::on_addStoreButton_clicked()
{
  addStore = new StoreWindow();
  connect(addStore, &StoreWindow::refresh, this, &DirectorWindow::refresh);
  addStore->show();
}


void DirectorWindow::on_removeStoreButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL RemoveStore(:id)}");
  query.bindValue(":id", storeModel_->index(currentIndex.row(), 0).data().toInt());

  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Нельзя удалить склад на котором есть книги!");
    msgBox.exec();
  }
  else
  {
    refresh();
  }
}


void DirectorWindow::on_storeTableView_clicked(const QModelIndex &index)
{
    currentIndex = index;
}


void DirectorWindow::on_storeTableView_doubleClicked(const QModelIndex &index)
{
  currentIndex = index;
  QSqlQuery query = QSqlQuery(db);
  query.prepare(QString("SELECT * FROM Store WHERE idStore = %1").arg(storeModel_->index(index.row(), 0).data().toInt()));
  query.exec();
  query.next();

  QSqlRecord record = query.record();

  int id = record.indexOf("idStore");
  int phone = record.indexOf("storePhone");
  int address = record.indexOf("storeAddress");

  addStore = new StoreWindow(
        record.value(id).toInt(),
        record.value(address).toString(),
        record.value(phone).toString());
  connect(addStore, &StoreWindow::refresh, this, &DirectorWindow::refresh);
  addStore->show();
}


void DirectorWindow::on_managerTableView_clicked(const QModelIndex &index)
{
    currentIndex = index;
}


void DirectorWindow::on_addManager_clicked()
{
  registration = new RegistrationWindow(false);
  connect(registration, &RegistrationWindow::refresh, this, &DirectorWindow::refresh);
  registration->show();
}


void DirectorWindow::on_removeManager_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL RemoveManager(:id)}");
  query.bindValue(":id", managerModel_->index(currentIndex.row(), 0).data().toInt());
  query.exec();
  query.next();
  refresh();
}


void DirectorWindow::on_managerTableView_doubleClicked(const QModelIndex &index)
{
    currentIndex = index;
    QSqlQuery query = QSqlQuery(db);
    query.prepare(QString("SELECT * FROM Manager WHERE idManager = %1").arg(managerModel_->index(index.row(), 0).data().toInt()));
    query.exec();
    query.next();

    QSqlRecord record = query.record();

    int name = record.indexOf("ManagerName");
    int phone = record.indexOf("ManagerPhone");
    int salary = record.indexOf("Salary");
    int id = record.indexOf("AuthId");

    QSqlQuery queryAuth = QSqlQuery(db);
    queryAuth.prepare(QString("SELECT * FROM Auth WHERE idAuth = %1").arg(record.value(id).toInt()));
    queryAuth.exec();
    queryAuth.next();

    QSqlRecord recordAuth = queryAuth.record();

    int login = recordAuth.indexOf("Login");

    qDebug() << managerModel_->index(currentIndex.row(), 0).data().toInt() << "passed";
    registration = new RegistrationWindow(
          record.value(name).toString(),
          record.value(phone).toString(),
          record.value(salary).toString(),
          recordAuth.value(login).toString(),
          managerModel_->index(currentIndex.row(), 0).data().toInt());
    connect(registration, &RegistrationWindow::refresh, this, &DirectorWindow::refresh);
    registration->show();
}

