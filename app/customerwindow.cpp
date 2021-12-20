#include "customerwindow.h"
#include "dbsingleton.h"
#include "ui_customerwindow.h"

#include <QMessageBox>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlRecord>

CustomerWindow::CustomerWindow(int id, QWidget *parent) :
  QDialog(parent),
  bookModel_(new QSqlQueryModel()),
  bookOrderModel_(new QSqlQueryModel()),
  orderModel_(new QSqlQueryModel()),
  ui(new Ui::CustomerWindow),
  customerId(id)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();

  bookModel_->setQuery(QString("SELECT DISTINCT idBook, BookTitle AS 'Название', AuthorName AS 'Автор' FROM Book "
                          "JOIN Author ON idAuthor = AuthorId "));
  ui->bookTableView->setModel(bookModel_);
  ui->bookTableView->resizeColumnsToContents();
  ui->bookTableView->setColumnHidden(0, true);

  ui->orderTableWidget->clearContents();
  ui->orderTableWidget->setRowCount(0);
  ui->orderTableWidget->setColumnCount(4);
  ui->orderTableWidget->setColumnHidden(0, true);

  ui->orderTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Название")));
  ui->orderTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("Цена")));
  ui->orderTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("Кол-во")));
  ui->orderTableWidget->resizeColumnToContents(2);
  ui->orderTableWidget->resizeColumnToContents(3);
  ui->orderTableWidget->setColumnWidth(1, 150);

  orderModel_->setQuery(QString("EXEC CustomerOrders %1").arg(customerId));
  ui->ordersTableView->setModel(orderModel_);
  ui->ordersTableView->setColumnHidden(0, true);
  ui->ordersTableView->resizeColumnsToContents();

  QSqlQuery query(db);
  query.exec(QString("SELECT * FROM Customer WHERE idCustomer = %1").arg(customerId));
  query.next();
  ui->name->setText(query.value(2).toString());
  ui->phone->setText(query.value(3).toString());
  ui->email->setText(query.value(4).toString());
  ui->dateEdit->setDate(query.value(5).toDate());

  QSqlQuery auth(db);
  auth.exec(QString("SELECT * FROM Auth WHERE idAuth = %1").arg(query.value(1).toInt()));
  auth.next();
  ui->login->setText(auth.value(1).toString());
  ui->password->setPlaceholderText("Введите новый пароль");
  ui->password->setEchoMode(QLineEdit::Password);
  ui->password2->setEchoMode(QLineEdit::Password);
  authId = auth.value(0).toInt();
}

CustomerWindow::~CustomerWindow()
{
  delete ui;
}

void CustomerWindow::on_LogOutButton_clicked()
{
  this->close();
  emit firstWindow();
}


void CustomerWindow::on_bookTableView_doubleClicked(const QModelIndex &index)
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


void CustomerWindow::on_searchLineEdit_textEdited(const QString &arg)
{
  if(arg != "")
  {
    bookModel_->setQuery(QString("EXEC SearchByWord %1").arg(arg));
    ui->bookTableView->setModel(bookModel_);
    ui->bookTableView->setColumnHidden(0, true);
  }
  else
  {
    bookModel_->setQuery(QString("SELECT DISTINCT idBook, BookTitle AS 'Название', AuthorName AS 'Автор' FROM Book "
                                 "JOIN Author ON idAuthor = AuthorId "));
    ui->bookTableView->setModel(bookModel_);
    ui->bookTableView->resizeColumnsToContents();
    ui->bookTableView->setColumnHidden(0, true);
  }
}


void CustomerWindow::on_bookTableView_clicked(const QModelIndex &index)
{
   currentIndex = index;
}


void CustomerWindow::on_addBookButton_clicked()
{
  QSqlQuery query(db);
  query.exec(QString("SELECT idBook, BookTitle AS 'Название', Price AS 'Цена'"
                     "FROM Book WHERE idBook = %1").arg(bookModel_->index(currentIndex.row(), 0).data().toInt()));

  while(query.next())
  {
    for (int i = 0; i < ui->orderTableWidget->rowCount(); i++)
    {
      QString id = ui->orderTableWidget->item(i, 0)->text();

      if (id == query.value(0).toString())
      {
        int inc =  ui->orderTableWidget->item(i, 3)->text().toInt() + 1;
        QTableWidgetItem *amount = new QTableWidgetItem(QString::number(inc));
        ui->orderTableWidget->setItem(i, 3, amount);
        return;
      }
    }
    int nRow = ui->orderTableWidget->rowCount();
    ui->orderTableWidget->insertRow(nRow);

    QTableWidgetItem *id = new QTableWidgetItem(query.value(0).toString());
    QTableWidgetItem *name = new QTableWidgetItem(query.value(1).toString());
    QTableWidgetItem *price = new QTableWidgetItem(query.value(2).toString());
    QTableWidgetItem *amount = new QTableWidgetItem(QString("1"));
    name->setFlags(name->flags() & ~Qt::ItemIsEditable);
    price->setFlags(price->flags() & ~Qt::ItemIsEditable);

    ui->orderTableWidget->setItem(nRow, 1, name);
    ui->orderTableWidget->setItem(nRow, 2, price);
    ui->orderTableWidget->setItem(nRow, 3, amount);
    ui->orderTableWidget->setItem(nRow, 0, id);
  }
}


void CustomerWindow::on_orderTableWidget_cellChanged(int row, int column)
{
  if (column == 3)
  {
    int sum = 0;
    for (int i = 0; i < ui->orderTableWidget->rowCount(); i++)
    {
      sum += ui->orderTableWidget->item(i, 2)->text().toInt() * ui->orderTableWidget->item(i, 3)->text().toInt();
    }
    ui->sum->setText(QString::number(sum));
  }
}


void CustomerWindow::on_orderButton_clicked()
{
  orderWindow = new OrderWindow(ui->orderTableWidget, ui->sum->text().toInt(), customerId);
  connect(orderWindow, &OrderWindow::clear, this, &CustomerWindow::clear);
  connect(orderWindow, &OrderWindow::refresh, this, &CustomerWindow::refresh);
  orderWindow->show();
}

void CustomerWindow::clear()
{
  ui->orderTableWidget->clearContents();
  ui->sum->setText("");
}

void CustomerWindow::refresh()
{
  orderModel_->setQuery(orderModel_->query().lastQuery());
  ui->ordersTableView->resizeColumnsToContents();
}


void CustomerWindow::on_regButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL UpdateCustomer(:id, :name, :phone, :email, :birth)}");
  query.bindValue(":name", ui->name->text());
  query.bindValue(":phone", ui->phone->text());
  query.bindValue(":id", customerId);
  query.bindValue(":email", ui->email->text());
  query.bindValue(":birth", ui->dateEdit->date().toString("yyyy-MM-dd"));
  query.exec();
  query.next();

  if (ui->password->text() != "")
  {
      QSqlQuery queryAuth = QSqlQuery(db);
      queryAuth.prepare("{ CALL UpdateAuth(:id, :login, :password)}");
      queryAuth.bindValue(":login", ui->login->text());
      queryAuth.bindValue(":password", ui->password->text());
      queryAuth.bindValue(":id", authId);
      queryAuth.exec();
      queryAuth.next();
  }
  QMessageBox msgBox;
  msgBox.setText("Данные сохранены!");
  msgBox.exec();
}


void CustomerWindow::on_ordersTableView_doubleClicked(const QModelIndex &index)
{
  orderItems = new OrderItemsWindow(orderModel_->index(index.row(), 0).data().toInt(), customerId);
  orderItems->show();
}

