#ifndef AUTHORWINDOW_H
#define AUTHORWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
  class AuthorWindow;
}

class AuthorWindow : public QDialog
{
  Q_OBJECT

public:
  explicit AuthorWindow(QWidget *parent = nullptr);
  AuthorWindow(int id, const QString& name, const QDate& birth, const QDate& death);
  ~AuthorWindow();

signals:
  void refresh();

protected:
  QSqlDatabase db;

private slots:
  void on_addButton_clicked();

  void on_checkBox_stateChanged(int arg1);

private:
  Ui::AuthorWindow *ui;
  bool isChanging = false;
  int currentId;
};

#endif // AUTHORWINDOW_H
