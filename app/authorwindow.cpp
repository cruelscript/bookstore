#include "authorwindow.h"
#include "dbsingleton.h"
#include "ui_authorwindow.h"

#include <QMessageBox>
#include <QSqlQuery>

AuthorWindow::AuthorWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AuthorWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();
  ui->death->setHidden(true);
}

AuthorWindow::AuthorWindow(int id,
                           const QString &name,
                           const QDate &birth,
                           const QDate &death):
  AuthorWindow()
{
  isChanging = true;
  currentId = id;
  ui->name->setText(name);
  ui->birth->setDate(birth);
  ui->checkBox->setChecked(true);
  ui->death->setHidden(false);
  ui->death->setDate(death);
}

AuthorWindow::~AuthorWindow()
{
  delete ui;
}

void AuthorWindow::on_addButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);

  isChanging
      ? query.prepare("{ CALL UpdateAuthor(:id, :name, :birth, :death)}")
      : query.prepare("{ CALL AddAuthor(:name, :birth, :death)}");
  query.bindValue(":name", ui->name->text());
  query.bindValue(":birth", ui->birth->date().toString("yyyy-MM-dd"));
  if (ui->death->date().toString("yyyy-MM-dd") == "2000-01-01")
  {
    query.bindValue(":death", "");
  }
  else
  {
    query.bindValue(":death", ui->death->date().toString("yyyy-MM-dd"));
  }
  query.bindValue(":id", currentId);

  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Такой автор уже добавлен в базу данных");
    msgBox.exec();
  }
  else
  {
    QMessageBox msgBox;
    msgBox.setText("Данные сохранены!");
    msgBox.exec();
    ui->name->setText("");
    this->close();
    emit refresh();
  }
}


void AuthorWindow::on_checkBox_stateChanged(int arg1)
{
  ui->checkBox->isChecked() ? ui->death->setHidden(false) : ui->death->setHidden(true);
}

