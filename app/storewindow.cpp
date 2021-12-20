#include "storewindow.h"
#include "ui_storewindow.h"
#include "dbsingleton.h"

#include <QMessageBox>
#include <QSqlQuery>

StoreWindow::StoreWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StoreWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();
  ui->phone->setInputMask("+7(000)0000000");
}

StoreWindow::StoreWindow(int id, const QString &address, const QString &phone) :
  StoreWindow()
{
  isChanging = true;
  currentId = id;
  ui->label->setText("Изменение данных склада");
  ui->phone->setText(phone);
  ui->address->setText(address);
  ui->changeButton->setText("Изменить");
}

StoreWindow::~StoreWindow()
{
  delete ui;
}

void StoreWindow::on_changeButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);

  isChanging
      ? query.prepare("{ CALL UpdateStore(:id, :address, :phone)}")
      : query.prepare("{ CALL AddStore(:address, :phone)}");
  query.bindValue(":address", ui->address->text());
  query.bindValue(":phone", ui->phone->text());
  query.bindValue(":id", currentId);

  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Такой склад уже добавлен в базу данных");
    msgBox.exec();
  }
  else
  {
    QMessageBox msgBox;
    msgBox.setText("Данные сохранены!");
    msgBox.exec();
    ui->address->setText("");
    ui->phone->setText("");
    this->close();
    emit refresh();
  }
}

