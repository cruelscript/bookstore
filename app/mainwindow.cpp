#include "mainwindow.h"
#include "managerwindow.h"
#include "directorwindow.h"
#include "customerwindow.h"
#include "dbsingleton.h"

#include "ui_mainwindow.h"
#include <iostream>
#include <QtSql/QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    db_ = DBSingleton::getInstance();
}

MainWindow::~MainWindow()
{
  delete ui;
  delete managerWindow;
  delete customerWindow;
  delete directorWindow;
}

MainWindow::Auth MainWindow::get()
{
  return {ui->loginEdit->text(), ui->passwordEdit->text()};
}

void MainWindow::on_AuthButton_clicked()
{
    Auth auth = get();
    QSqlQuery query = QSqlQuery(db_);
    query.prepare("{CALL Authorize(:login, :password)}");

    query.bindValue(":login", auth.login);
    query.bindValue(":password", auth.password);

    if (!query.exec())
    {
      QMessageBox msgBox;
      msgBox.setText("Неправильно введены учетные данные!");
      msgBox.exec();
    }

    qDebug() << (query.exec() ? "ok": "ne ok");
    query.next();
    QSqlRecord record = query.record();
    int authId = record.indexOf("id");
    int job = record.indexOf("job");
    AuthResult result = {record.value(authId).toString(), record.value(job).toString()};

    if (result.role == "Director")
    {
      directorWindow = new DirectorWindow();
      connect(directorWindow, &DirectorWindow::firstWindow, this, &MainWindow::show);
      this->close();
      directorWindow->show();
    }
    else if (result.role == "Manager")
    {
      managerWindow = new ManagerWindow();
      connect(managerWindow, &ManagerWindow::firstWindow, this, &MainWindow::show);
      this->close();
      managerWindow->show();
    }
    else if (result.role == "Customer")
    {
      customerWindow = new CustomerWindow(result.id.toInt());
      connect(customerWindow, &CustomerWindow::firstWindow, this, &MainWindow::show);
      this->close();
      customerWindow->show();
    }
    ui->loginEdit->setText("");
    ui->passwordEdit->setText("");
}

void MainWindow::on_registrationButton_clicked()
{
  this->close();
  registrationWindow = new RegistrationWindow(true);
  connect(registrationWindow, &RegistrationWindow::firstWindow, this, &MainWindow::show);
  registrationWindow->show();
}

