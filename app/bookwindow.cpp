#include "bookwindow.h"
#include "dbsingleton.h"
#include "ui_bookwindow.h"

#include <QMessageBox>
#include <QSqlQuery>

BookWindow::BookWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BookWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();

  QSqlQuery author(db);
  author.exec(QString("SELECT * FROM Author"));

  int count = 0;
  while(author.next())
  {
    ui->author->insertItem(count, author.value(1).toString());
    authors[author.value(1).toString()] = author.value(0).toInt();
    count++;
  }

  QSqlQuery publisher(db);
  publisher.exec(QString("SELECT * FROM Publisher"));

  count = 0;
  while(publisher.next())
  {
    ui->publisher->insertItem(count, publisher.value(1).toString());
    publishers[publisher.value(1).toString()] = publisher.value(0).toInt();
    count++;
  }
}

BookWindow::BookWindow(int id,
                       const QString &title,
                       const QString &year,
                       int publisherId,
                       int authorId,
                       const QString &price,
                       const QString &description) :
  BookWindow()
{
  ui->amount->setHidden(true);
  isChanging = true;
  currentId = id;
  ui->title->setText(title);
  ui->year->setText(year);
  for (auto it = authors.begin(); it != authors.end(); ++it)
      if (it->second == authorId)
           ui->author->setCurrentText(it->first);
  for (auto it = publishers.begin(); it != publishers.end(); ++it)
      if (it->second == publisherId)
           ui->publisher->setCurrentText(it->first);
  ui->price->setText(price);
  ui->description->document()->setPlainText(description);
}

BookWindow::~BookWindow()
{
  delete ui;
}

void BookWindow::on_addButton_clicked()
{
  QSqlQuery query(db);

  isChanging
      ? query.prepare("{ CALL UpdateBook(:id, :title, :year, :publisherId, :authorId, :price, :description)}")
      : query.prepare("{ CALL AddBook(:title, :year, :publisherId, :authorId, :price, :description)}");
  query.bindValue(":title", ui->title->text());
  query.bindValue(":year", ui->year->text().toInt());
  query.bindValue(":publisherId", publishers[ui->publisher->currentText()]);
  query.bindValue(":authorId", authors[ui->author->currentText()]);
  query.bindValue(":price", ui->price->text().toInt());
  query.bindValue(":description", ui->description->toPlainText());
  query.bindValue(":id", currentId);

  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Такая книга уже добавлена в базу данных");
    msgBox.exec();
  }
  else
  {
    if(!isChanging)
    {
      query.next();
      qDebug() << query.value(0);
      QSqlQuery addAmount(db);
      addAmount.exec(QString("INSERT INTO BookStore VALUES (%1, 1, %2)").arg(query.value(0).toInt()).arg(ui->amount->text().toInt()));
    }
    QMessageBox msgBox;
    msgBox.setText("Данные сохранены!");
    msgBox.exec();
    ui->title->setText("");
    ui->year->setText("");
    ui->price->setText("");
    ui->description->document()->setPlainText("");
    this->close();
    emit refresh();
  }
}

