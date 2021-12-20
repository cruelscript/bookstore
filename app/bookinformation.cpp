#include "bookinformation.h"
#include "ui_bookinformation.h"

BookInformation::BookInformation(
    const QString &title,
    const QString &author,
    const QString &pub,
    const QString &year,
    const QString &price,
    const QString &description,
    QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BookInformation)
{
  ui->setupUi(this);
  ui->bookTitle->setText(title);
  ui->author->setText(author);
  ui->publisher->setText(pub);
  ui->year->setText(year);
  ui->price->setText(price);
  ui->description->setText(description);
}

BookInformation::~BookInformation()
{
  delete ui;
}
