#include "registrationwindow.h"
#include "ui_registrationwindow.h"

#include <QMessageBox>
#include <QSqlQuery>


RegistrationWindow::RegistrationWindow(bool isCustomer, QWidget *parent):
  QDialog(parent),
  ui(new Ui::RegistrationWindow),
  isChanging_(false),
  isCustomer_(isCustomer)
{
  ui->setupUi(this);
  ui->phone->setInputMask("+7(000)0000000");
  ui->password->setEchoMode(QLineEdit::Password);
  ui->password2->setEchoMode(QLineEdit::Password);

  if (isCustomer)
  {
    ui->salary->setHidden(true);
    ui->email->setHidden(false);
    ui->birthLabel->setHidden(false);
    ui->dateEdit->setHidden(false);
  }
  else
  {
    ui->email->setHidden(true);
    ui->birthLabel->setHidden(true);
    ui->dateEdit->setHidden(true);
    ui->salary->setHidden(false);
  }
}

RegistrationWindow::RegistrationWindow(QString name,
                                       QString phone,
                                       QString email,
                                       QString birthdate,
                                       QString login,
                                       int id,
                                       QWidget *parent) :
  RegistrationWindow(true)
{
  isChanging_ = true;
  currentId = id;
  ui->name->setText(name);
  ui->phone->setText(phone);
  ui->email->setText(email);
  ui->dateEdit->setDate(stringTodate(birthdate));
  ui->login->setText(login);
  ui->password->setPlaceholderText("Введите новый пароль");
  ui->regButton->setText("Сохранить изменения");
}

RegistrationWindow::RegistrationWindow(QString name,
                                       QString phone,
                                       QString salary,
                                       QString login,
                                       int id,
                                       QWidget *parent) :
  RegistrationWindow(false)
{
  isChanging_ = true;
  currentId = id;
  ui->name->setText(name);
  ui->phone->setText(phone);
  ui->salary->setText(salary);
  ui->login->setText(login);
  ui->password->setPlaceholderText("Введите новый пароль");
  ui->regButton->setText("Сохранить изменения");
}

RegistrationWindow::~RegistrationWindow()
{
  delete ui;
}

QDate RegistrationWindow::stringTodate(const QString &string)
{
  return QDate();
}

void RegistrationWindow::on_regButton_clicked()
{
  if(isChanging_ &&isCustomer_)
  {
    return;
  }
  if (!isChanging_ && isCustomer_)
  {
      QSqlQuery query = QSqlQuery(db);
      query.prepare("{ CALL AddCustomer(:name, :phone, :email, :date, :login, :password)}");
      query.bindValue(":name", ui->name->text());
      query.bindValue(":phone", ui->phone->text());
      query.bindValue(":email", ui->email->text());
      query.bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
      query.bindValue(":login", ui->login->text());
      if(ui->password->text() == ui->password2->text())
      {
          query.bindValue(":password", ui->password->text());
          query.exec();
          query.next();
          QMessageBox msgBox;
          msgBox.setText("Вы успешно зарегистрировались!");
          msgBox.exec();
          this->close();
          emit firstWindow();
          return;
      }
      else
      {
          QMessageBox msgBox;
          msgBox.setText("Пароли не совпадают!");
          msgBox.exec();
      }
  }
  if (isChanging_ && !isCustomer_)
  {
      QSqlQuery query = QSqlQuery(db);
      query.prepare("{ CALL UpdateManager(:id, :name, :phone)}");
      query.bindValue(":name", ui->name->text());
      query.bindValue(":phone", ui->phone->text());
      query.bindValue(":id", currentId);
      query.exec();
      query.next();

      QSqlQuery querySalary = QSqlQuery(db);
      querySalary.prepare("{ CALL UpdateManagerSalary(:id, :salary)}");
      querySalary.bindValue(":salary", ui->salary->text());
      querySalary.bindValue(":id", currentId);
      querySalary.exec();
      querySalary.next();

      if (ui->password->text() != "")
      {
          QSqlQuery queryAuth = QSqlQuery(db);
          queryAuth.prepare("{ CALL UpdateAuth(:id, :login, :password)}");
          queryAuth.bindValue(":login", ui->login->text());
          queryAuth.bindValue(":password", ui->password->text());
          queryAuth.bindValue(":id", currentId);
          queryAuth.exec();
          queryAuth.next();
      }
      QMessageBox msgBox;
      msgBox.setText("Данные сохранены!");
      msgBox.exec();
      this->close();
      emit refresh();
      return;
  }
  if (!isChanging_ && !isCustomer_)
  {
      QSqlQuery query = QSqlQuery(db);
      query.prepare("{ CALL AddManager(:name, :phone, :salary, :login, :password)}");
      query.bindValue(":name", ui->name->text());
      query.bindValue(":phone", ui->phone->text());
      query.bindValue(":salary", ui->salary->text());
      query.bindValue(":login", ui->login->text());
      query.bindValue(":password", ui->password->text());
      query.exec();
      query.next();
      QMessageBox msgBox;
      msgBox.setText("Менеджер добавлен!");
      msgBox.exec();
      this->close();
      emit refresh();
      return;
  }
}

