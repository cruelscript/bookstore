#ifndef DBSINGLETON_H
#define DBSINGLETON_H

#include <QSqlDatabase>

class DBSingleton
{
public:
  static QSqlDatabase& getInstance() {
    static DBSingleton instance;
    return instance.db_;
  }
protected:
  DBSingleton();
  DBSingleton(DBSingleton&) = delete;
  void operator=(const DBSingleton&) = delete;

private:
  QSqlDatabase db_;
};

#endif // DBSINGLETON_H
