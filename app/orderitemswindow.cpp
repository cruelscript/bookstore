#include "dbsingleton.h"
#include "orderitemswindow.h"
#include "ui_orderitemswindow.h"

OrderItemsWindow::OrderItemsWindow(int idOrder, int idCustomer, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::OrderItemsWindow),
  orderItemsModel_(new QSqlQueryModel),
  customerId(idCustomer),
  orderId(idOrder)
{
  ui->setupUi(this);
  db = DBSingleton::getInstance();

  orderItemsModel_->setQuery(QString("SELECT BookTitle AS 'Название', AuthorName AS 'Автор', Price AS 'Цена', Amount AS 'Кол-во' FROM Book "
                                     "  JOIN Author ON idAuthor = authorId "
                                     "  JOIN BookOrder ON idBook = BookId "
                                     "  JOIN [dbo].[Order] ON idOrder = OrderId "
                                     "    WHERE CustomerId = %1 AND idOrder = %2").arg(customerId).arg(orderId));
  qDebug() << customerId << orderId;
  ui->ordersTableView->setModel(orderItemsModel_);
  ui->ordersTableView->resizeColumnsToContents();
}

OrderItemsWindow::~OrderItemsWindow()
{
  delete ui;
}
