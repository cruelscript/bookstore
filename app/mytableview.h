#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QTableView>

class MyTableView : public QTableView
{
   Q_OBJECT

public:
  explicit MyTableView(QWidget *parent = 0);

signals:
  void rowChanged(int nRowCurrent, int nRowPrevious);

protected:
  void currentChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // MYTABLEVIEW_H
