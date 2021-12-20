#include "dbsingleton.h"
#include "publisherwindow.h"
#include "ui_publisherwindow.h"

#include <QMessageBox>
#include <QSqlQuery>

PublisherWindow::PublisherWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PublisherWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();
  ui->phone->setInputMask("+7(000)0000000");
}

PublisherWindow::PublisherWindow(int id,
                                 const QString &name,
                                 const QString &address,
                                 const QString &phone):
  PublisherWindow()
{
  isChanging = true;
  currentId = id;
  ui->name->setText(name);
  ui->address->setText(address);
  ui->phone->setText(phone);
}

PublisherWindow::~PublisherWindow()
{
  delete ui;
}

void PublisherWindow::on_addButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);

  qDebug() << isChanging;
  isChanging
      ? query.prepare("{ CALL UpdatePublisher(:id, :name, :address, :phone)}")
      : query.prepare("{ CALL AddPublisher(:name, :address, :phone)}");
  query.bindValue(":name", ui->name->text());
  query.bindValue(":address", ui->address->text());
  query.bindValue(":phone", ui->phone->text());
  query.bindValue(":id", currentId);

  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Такой издатель уже добавлен в базу данных");
    msgBox.exec();
  }
  else
  {
      qDebug() << "ne ok";
    QMessageBox msgBox;
    msgBox.setText("Данные сохранены!");
    msgBox.exec();
    ui->name->setText("");
    ui->address->setText("");
    ui->phone->setText("");
    this->close();
    emit refresh();
  }
}

