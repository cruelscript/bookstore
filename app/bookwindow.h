#ifndef BOOKWINDOW_H
#define BOOKWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
  class BookWindow;
}

class BookWindow : public QDialog
{
  Q_OBJECT

public:
  explicit BookWindow(QWidget *parent = nullptr);
  BookWindow(int id,
             const QString& title,
             const QString& year,
             int publisherId,
             int authorId,
             const QString& price,
             const QString& description);
  ~BookWindow();

signals:
  void refresh();

protected:
  QSqlDatabase db;

private slots:
  void on_addButton_clicked();

private:
  Ui::BookWindow *ui;
  std::map<QString, int> authors;
  std::map<QString, int> publishers;
  bool isChanging = false;
  int currentId;
};

#endif // BOOKWINDOW_H
