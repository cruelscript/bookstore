#ifndef BOOKINFORMATION_H
#define BOOKINFORMATION_H

#include <QDialog>

namespace Ui {
  class BookInformation;
}

class BookInformation : public QDialog
{
  Q_OBJECT

public:
  BookInformation(const QString &title = "",
                  const QString &author = "",
                  const QString &pub = "",
                  const QString &year = "",
                  const QString &price = "",
                  const QString &description = "",
                  QWidget *parent = nullptr);

  ~BookInformation();

signals:
  void returnWindow();

private:
  Ui::BookInformation *ui;
};

#endif // BOOKINFORMATION_H
