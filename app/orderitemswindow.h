#ifndef ORDERITEMSWINDOW_H
#define ORDERITEMSWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
  class OrderItemsWindow;
}

class OrderItemsWindow : public QDialog
{
  Q_OBJECT

public:
  explicit OrderItemsWindow(int, int, QWidget *parent = nullptr);
  ~OrderItemsWindow();

protected:
  QSqlDatabase db;

private:
  Ui::OrderItemsWindow *ui;
  QSqlQueryModel *orderItemsModel_;
  int customerId;
  int orderId;
};

#endif // ORDERITEMSWINDOW_H
