#ifndef ADDSTOREWINDOW_H
#define ADDSTOREWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
  class StoreWindow;
}

class StoreWindow : public QDialog
{
  Q_OBJECT

public:
  explicit StoreWindow(QWidget *parent = nullptr);
  StoreWindow(int id, const QString& address, const QString& phone);
  ~StoreWindow();

signals:
  void refresh();

protected:
  QSqlDatabase db;

private slots:
  void on_changeButton_clicked();

private:
  Ui::StoreWindow *ui;
  bool isChanging = false;
  int currentId;
};

#endif // ADDSTOREWINDOW_H
