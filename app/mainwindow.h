#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customerwindow.h"
#include "directorwindow.h"
#include "managerwindow.h"
#include "mytableview.h"
#include "registrationwindow.h"


#include <QMainWindow>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  struct Auth {
    QString login;
    QString password;
  };
  struct AuthResult {
    QString id;
    QString role;
  };
  enum Role {
    DIRECTOR,
    MANAGER,
    CUSTOMER
  };

  Auth get();
  void setAuthResult(AuthResult&);

protected:
  QSqlDatabase db_;

private slots:
  void on_AuthButton_clicked();
  void on_registrationButton_clicked();

private:
  Ui::MainWindow *ui;
  ManagerWindow *managerWindow;
  DirectorWindow *directorWindow;
  CustomerWindow *customerWindow;
  RegistrationWindow *registrationWindow;
};
#endif // MAINWINDOW_H
