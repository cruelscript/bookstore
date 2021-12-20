#ifndef CUSTOMERWINDOW_H
#define CUSTOMERWINDOW_H

#include "bookinformation.h"
#include "orderitemswindow.h"
#include "orderwindow.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
  class CustomerWindow;
}

class CustomerWindow : public QDialog
{
  Q_OBJECT

public:
  explicit CustomerWindow(int id, QWidget *parent = nullptr);
  ~CustomerWindow();

private slots:
  void on_LogOutButton_clicked();

  void on_bookTableView_doubleClicked(const QModelIndex &index);

  void on_searchLineEdit_textEdited(const QString &arg1);

  void on_bookTableView_clicked(const QModelIndex &index);

  void on_addBookButton_clicked();

  void on_orderTableWidget_cellChanged(int row, int column);

  void on_orderButton_clicked();

  void on_regButton_clicked();

  void on_ordersTableView_doubleClicked(const QModelIndex &index);

signals:
  void firstWindow();

protected:
  QSqlDatabase db;


private:
  QSqlQueryModel *bookModel_;
  QSqlQueryModel *bookOrderModel_;
  QSqlQueryModel *orderModel_;

  BookInformation *bookInformation;
  OrderWindow *orderWindow;
  OrderItemsWindow *orderItems;

  QModelIndex currentIndex;
  void clear();
  void refresh();

  Ui::CustomerWindow *ui;
  int customerId;
  int authId;
};

#endif // CUSTOMERWINDOW_H
