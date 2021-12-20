#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
  class RegistrationWindow;
}

class RegistrationWindow : public QDialog
{
  Q_OBJECT

public:
  explicit RegistrationWindow(bool isCustomer, QWidget *parent = nullptr);
  RegistrationWindow(QString name,
                     QString phone,
                     QString email,
                     QString birthdate,
                     QString login,
                     int id,
                     QWidget *parent = nullptr);

  RegistrationWindow(QString name,
                     QString phone,
                     QString salary,
                     QString login,
                     int id,
                     QWidget *parent = nullptr);

  ~RegistrationWindow();

  static QDate stringTodate(const QString&);

signals:
  void refresh();
  void firstWindow();

protected:
  QSqlDatabase db;

private slots:
  void on_regButton_clicked();

private:
  Ui::RegistrationWindow *ui;
  bool isChanging_ = false;
  bool isCustomer_ = true;
  int currentId;
};

#endif // REGISTRATIONWINDOW_H
