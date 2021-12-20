#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include "authorwindow.h"
#include "bookinformation.h"
#include "bookwindow.h"
#include "mytableview.h"
#include "publisherwindow.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
  class ManagerWindow;
}

class ManagerWindow : public QDialog
{
  Q_OBJECT

public:
  explicit ManagerWindow(QWidget *parent = nullptr);
  ~ManagerWindow();

signals:
  void firstWindow();

private slots:
  void on_LogOutButton_clicked();

  void on_bookTableView_doubleClicked(const QModelIndex &index);

  void on_addBookButton_clicked();

  void on_AuthorTableView_clicked(const QModelIndex &index);

  void on_AuthorTableView_doubleClicked(const QModelIndex &index);

  void on_addAuthorButton_clicked();

  void on_removeAuthorButton_clicked();

  void on_publisherTableView_clicked(const QModelIndex &index);

  void on_publisherTableView_doubleClicked(const QModelIndex &index);

  void on_addPublisherButton_clicked();

  void on_removePublisherButton_clicked();

  void on_store_currentTextChanged(const QString &arg1);

  void on_bookStoreTableView_doubleClicked(const QModelIndex &index);

protected:
  QSqlDatabase db;

private:
  QSqlQueryModel *bookModel_;
  QSqlQueryModel *authorModel_;
  QSqlQueryModel *publisherModel_;
  QSqlQueryModel *bookStoreModel_;
  QSqlQueryModel *OrderStoreModel_;

  BookInformation *bookInformation;
  BookWindow *bookWindow;
  AuthorWindow *authorWindow;
  PublisherWindow *publisherWindow;

  QModelIndex currentIndex;
  void refresh();
  std::map<QString, int> stores;

  Ui::ManagerWindow *ui;

};

#endif // MANAGERWINDOW_H
