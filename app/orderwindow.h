#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QTableWidget>

namespace Ui {
  class OrderWindow;
}

class OrderWindow : public QDialog
{
  Q_OBJECT

public:
  explicit OrderWindow(QTableWidget* data, int sum, int customerId, QWidget *parent = nullptr);
  ~OrderWindow();

signals:
  void firstWindow();
  void clear();
  void refresh();

private slots:
  void on_orderButton_clicked();

protected:
  QSqlDatabase db;

private:
  Ui::OrderWindow *ui;
  std::map<QString, int> stores;
  int customerId;
};

#endif // ORDERWINDOW_H
