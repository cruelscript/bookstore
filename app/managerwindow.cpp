#include "managerwindow.h"
#include "ui_managerwindow.h"
#include "dbsingleton.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QInputDialog>

ManagerWindow::ManagerWindow(QWidget *parent) :
  QDialog(parent),
  bookModel_(new QSqlQueryModel()),
  authorModel_(new QSqlQueryModel()),
  publisherModel_(new QSqlQueryModel()),
  bookStoreModel_(new QSqlQueryModel()),
  OrderStoreModel_(new QSqlQueryModel()),
  ui(new Ui::ManagerWindow)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();

  bookModel_->setQuery(QString("SELECT * FROM BookPopularity"));
  ui->bookTableView->setModel(bookModel_);
  ui->bookTableView->resizeColumnsToContents();
  ui->bookTableView->setColumnHidden(0, true);

  authorModel_->setQuery(QString("SELECT idAuthor, "
                                 "AuthorName AS 'Имя автора', "
                                 "AuthorBirthDate AS 'Дата рождения', "
                                 "AuthorDeathDate AS 'Дата смерти' FROM Author"));
  ui->AuthorTableView->setModel(authorModel_);
  ui->AuthorTableView->resizeColumnsToContents();
  ui->AuthorTableView->setColumnHidden(0, true);

  publisherModel_->setQuery(QString("SELECT idPublisher, "
                                    "PubName AS 'Название', "
                                    "PubAddress AS 'Адрес', "
                                    "PubPhone AS 'Телефон' FROM Publisher"));
  ui->publisherTableView->setModel(publisherModel_);
  ui->publisherTableView->resizeColumnsToContents();
  ui->publisherTableView->setColumnHidden(0, true);

  QSqlQuery query(db);
  query.exec(QString("SELECT * FROM Store"));

  int count  = 0;
  while(query.next())
  {
    ui->store->insertItem(count, query.value(1).toString());
    stores[query.value(1).toString()] = query.value(0).toInt();
    count++;
  }
}

ManagerWindow::~ManagerWindow()
{
  delete ui;
  delete bookModel_;
  delete authorModel_;
  delete publisherModel_;
}


void ManagerWindow::on_LogOutButton_clicked()
{
  this->close();
  emit firstWindow();
}

void ManagerWindow::refresh()
{
  bookModel_->setQuery(bookModel_->query().lastQuery());
  authorModel_->setQuery(authorModel_->query().lastQuery());
  publisherModel_->setQuery(publisherModel_->query().lastQuery());
  bookStoreModel_->setQuery(bookStoreModel_->query().lastQuery());
  OrderStoreModel_->setQuery(OrderStoreModel_->query().lastQuery());

  ui->bookTableView->resizeColumnsToContents();
  ui->AuthorTableView->resizeColumnsToContents();
  ui->publisherTableView->resizeColumnsToContents();
  ui->bookStoreTableView->resizeColumnsToContents();
  ui->orderStoreTableView->resizeColumnsToContents();
}


void ManagerWindow::on_bookTableView_doubleClicked(const QModelIndex &index)
{
  currentIndex = index;
  QSqlQuery query(db);
  query.exec(QString("SELECT * FROM Book Where idBook = %1").arg(bookModel_->index(currentIndex.row(), 0).data().toInt()));
  query.next();
  bookWindow = new BookWindow(query.value(0).toInt(),
                              query.value(1).toString(),
                              query.value(2).toString(),
                              query.value(3).toInt(),
                              query.value(4).toInt(),
                              query.value(5).toString(),
                              query.value(6).toString()
                             );
  connect(bookWindow, &BookWindow::refresh, this, &ManagerWindow::refresh);
  bookWindow->show();
}


void ManagerWindow::on_addBookButton_clicked()
{
  bookWindow = new BookWindow();
  connect(bookWindow, &BookWindow::refresh, this, &ManagerWindow::refresh);
  bookWindow->show();
}


void ManagerWindow::on_AuthorTableView_clicked(const QModelIndex &index)
{
  currentIndex = index;
}


void ManagerWindow::on_AuthorTableView_doubleClicked(const QModelIndex &index)
{
  currentIndex = index;
  authorWindow = new AuthorWindow(authorModel_->index(index.row(), 0).data().toInt(),
                                  authorModel_->index(index.row(), 1).data().toString(),
                                  authorModel_->index(index.row(), 2).data().toDate(),
                                  authorModel_->index(index.row(), 3).data().toDate());
  connect(authorWindow, &AuthorWindow::refresh, this, &ManagerWindow::refresh);
  authorWindow->show();
}


void ManagerWindow::on_addAuthorButton_clicked()
{
  authorWindow = new AuthorWindow();
  connect(authorWindow, &AuthorWindow::refresh, this, &ManagerWindow::refresh);
  authorWindow->show();
}


void ManagerWindow::on_removeAuthorButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL RemoveAuthor(:id)}");
  query.bindValue(":id", authorModel_->index(currentIndex.row(), 0).data().toInt());
  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Нельзя удалить автора, у которого есть книги!");
    msgBox.exec();
  }
  else
  {
    refresh();
  }
}


void ManagerWindow::on_publisherTableView_clicked(const QModelIndex &index)
{
  currentIndex = index;
}


void ManagerWindow::on_publisherTableView_doubleClicked(const QModelIndex &index)
{
  currentIndex = index;
  publisherWindow = new PublisherWindow(publisherModel_->index(index.row(), 0).data().toInt(),
                                        publisherModel_->index(index.row(), 1).data().toString(),
                                        publisherModel_->index(index.row(), 2).data().toString(),
                                        publisherModel_->index(index.row(), 3).data().toString());
  connect(publisherWindow, &PublisherWindow::refresh, this, &ManagerWindow::refresh);
  publisherWindow->show();
}


void ManagerWindow::on_addPublisherButton_clicked()
{
  publisherWindow = new PublisherWindow();
  connect(publisherWindow, &PublisherWindow::refresh, this, &ManagerWindow::refresh);
  publisherWindow->show();
}


void ManagerWindow::on_removePublisherButton_clicked()
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare("{ CALL RemovePublisher(:id)}");
  query.bindValue(":id", publisherModel_->index(currentIndex.row(), 0).data().toInt());
  if(!query.exec())
  {
    QMessageBox msgBox;
    msgBox.setText("Нельзя удалить издателя, у которого есть книги!");
    msgBox.exec();
  }
  else
  {
    refresh();
  }
}


void ManagerWindow::on_store_currentTextChanged(const QString &arg1)
{
  bookStoreModel_->setQuery(QString("EXEC StoreBooks %1").arg(stores[arg1]));
  ui->bookStoreTableView->setModel(bookStoreModel_);
  ui->bookStoreTableView->setColumnHidden(0, true);
  ui->bookStoreTableView->resizeColumnsToContents();

  OrderStoreModel_->setQuery(QString("EXEC OrderStore %1").arg(stores[arg1]));
  ui->orderStoreTableView->setModel(OrderStoreModel_);
  ui->orderStoreTableView->resizeColumnsToContents();
  ui->orderStoreTableView->setColumnHidden(0, true);
}


void ManagerWindow::on_bookStoreTableView_doubleClicked(const QModelIndex &index)
{
  QSqlQuery query = QSqlQuery(db);
  query.prepare(QString("SELECT * FROM BookStore WHERE storeId = %1 AND bookId = %2")
                .arg(stores[ui->store->currentText()])
                .arg(bookStoreModel_->index(index.row(), 0).data().toInt()));
  query.exec();
  query.next();

  QInputDialog input;
  QString newAmount = QInputDialog::getText(this,
                                           "Change Amount",
                                           "Изменить количество книг",
                                           QLineEdit::Normal,
                                           query.value(3).toString());

  query.prepare(QString("EXEC UpdateAmount %1, %2, %3")
                .arg(bookStoreModel_->index(index.row(), 3).data().toInt() - newAmount.toInt())
                .arg(bookStoreModel_->index(index.row(), 0).data().toInt())
                .arg(stores[ui->store->currentText()]));

  if (newAmount.toInt() > 0)
  {
      query.exec();
  }
  refresh();
}

