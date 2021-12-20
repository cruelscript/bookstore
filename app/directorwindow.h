#ifndef DIRECTORWINDOW_H
#define DIRECTORWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "storewindow.h"
#include "bookinformation.h"
#include "registrationwindow.h"

namespace Ui {
  class DirectorWindow;
}

class DirectorWindow : public QDialog
{
  Q_OBJECT

public:
  explicit DirectorWindow(QWidget *parent = nullptr);
  ~DirectorWindow();

  void refresh();

signals:
  void firstWindow();
  void bookWindow();

private slots:
  void on_LogOutButton_clicked();

  void on_bookTableView_clicked(const QModelIndex &index);

  void on_bookTableView_doubleClicked(const QModelIndex &index);

  void on_changePriceButton_clicked();

  void on_removeBookButton_clicked();

  void on_addStoreButton_clicked();

  void on_removeStoreButton_clicked();

  void on_storeTableView_clicked(const QModelIndex &index);

  void on_storeTableView_doubleClicked(const QModelIndex &index);

  void on_managerTableView_clicked(const QModelIndex &index);

  void on_addManager_clicked();

  void on_removeManager_clicked();

  void on_managerTableView_doubleClicked(const QModelIndex &index);

protected:
  QSqlDatabase db;

  QSqlQueryModel *bookModel_;
  QSqlQueryModel *storeModel_;
  QSqlQueryModel *managerModel_;

  BookInformation *bookInformation;
  RegistrationWindow *registration;
  StoreWindow *addStore;

  QModelIndex currentIndex;

private:
  Ui::DirectorWindow *ui;

};

#endif // DIRECTORWINDOW_H
