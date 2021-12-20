#include "dbsingleton.h"
#include "orderwindow.h"
#include "ui_orderwindow.h"

#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

OrderWindow::OrderWindow(QTableWidget* data, int sum, int id, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::OrderWindow)
{
  qDebug() << data->itemAt(0, 0)->text();
  ui->setupUi(this);
  ui->orderTableWidget->clearContents();
  ui->orderTableWidget->setRowCount(data->rowCount());
  ui->orderTableWidget->setColumnCount(data->columnCount());
  ui->orderTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Название")));
  ui->orderTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(QString("Цена")));
  ui->orderTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(QString("Кол-во")));
  ui->orderTableWidget->setColumnHidden(0, true);

  for(int i = 0; i < ui->orderTableWidget->rowCount(); i++)
  {
    for(int j = 0; j < ui->orderTableWidget->columnCount(); j++)
    {
      qDebug() <<  data->item(i, j)->text();
      ui->orderTableWidget->setItem(i, j, new QTableWidgetItem(data->item(i, j)->text()));
    }
  }
  ui->orderTableWidget->resizeColumnsToContents();

  QSqlQuery query(db);
  query.exec(QString("SELECT * FROM Store"));


  int count  = 0;
  while(query.next())
  {
    qDebug() << query.value(1).toString();
    ui->comboBox->insertItem(count, query.value(1).toString());
    stores[query.value(1).toString()] = query.value(0).toInt();
    count++;
  }

  ui->sum->setText(QString::number(sum));
  customerId = id;
}


OrderWindow::~OrderWindow()
{
  delete ui;
}

void OrderWindow::on_orderButton_clicked()
{
  QSqlQuery query(db);
  query.prepare("EXEC AddOrder :id, :store, :date, :price");

  query.bindValue(":id", customerId);
  query.bindValue(":store", stores[ui->comboBox->currentText()]);
  query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
  query.bindValue(":price", ui->sum->text());

  query.exec();
  query.next();
  int orderId = query.value(0).toInt();

  QSqlQuery addBooks(db);
  addBooks.prepare("INSERT INTO BookOrder VALUES (:orderId, :bookId, :amount)");
  addBooks.bindValue(":orderId", orderId);

  int bookId = 0;
  int amount = 0;


  for (int i = 0; i < ui->orderTableWidget->rowCount(); i++)
  {
    bookId = ui->orderTableWidget->item(i, 0)->text().toInt();
    amount = ui->orderTableWidget->item(i, 3)->text().toInt();
    addBooks.bindValue(":bookId", bookId);
    addBooks.bindValue(":amount", amount);
    QSqlQuery changeAmount(db);
    changeAmount.prepare(QString("EXEC UpdateAmount %1, %2, %3")
                         .arg(amount)
                         .arg(bookId)
                         .arg(stores[ui->comboBox->currentText()]));
    addBooks.exec();
    qDebug() << amount << bookId << stores[ui->comboBox->currentText()];
    changeAmount.exec();
  }
  QMessageBox msgBox;
  msgBox.setText("Заказ оформлен!");
  msgBox.exec();
  this->close();
  emit clear();
  emit refresh();
}

