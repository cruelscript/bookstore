#ifndef PUBLISHERWINDOW_H
#define PUBLISHERWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
  class PublisherWindow;
}

class PublisherWindow : public QDialog
{
  Q_OBJECT

public:
  explicit PublisherWindow(QWidget *parent = nullptr);
  PublisherWindow(int id, const QString& name, const QString& address, const QString& phone);
  ~PublisherWindow();

signals:
  void refresh();

protected:
  QSqlDatabase db;

private slots:
  void on_addButton_clicked();

private:
  Ui::PublisherWindow *ui;
  bool isChanging = false;
  int currentId;
};

#endif // PUBLISHERWINDOW_H
