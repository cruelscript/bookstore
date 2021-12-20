#include "mytableview.h"

MyTableView::MyTableView(QWidget *parent) :
  QTableView(parent)
{}

void MyTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
  if (previous.row() != current.row()) emit rowChanged(current.row(), previous.row());
}

