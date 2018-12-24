
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "pdf-pull/qlispconsole-dialog.h"
#include "pdf-pull/qlispconsole.h"

#include "pdf-pull/pdf-pull.h"

#include <QApplication>

#include <QVector>

#include "pin.h"

void test(pinned(int ,x))
{
 _repin(x) _as(x + 8);
 qDebug() << "x = " << nip(x);
}

void demo1()
{
 int a = 9;
 _pin(int ,x) _tobe(a + 3);
 a += 20;
 test(x_pinned);
}

void demo2()
{
 int a = 9;

 _pin(int ,x) _tobe(a + 3);

 int y = nip(x);
 _repin(x) _as(x + 5);

 qDebug() << "y = " << y;
 qDebug() << "x = " << nip(x);
}

void demo3()
{
 int a = 9;

 _pin(int ,x) _tobe(a + 3);
 _pin(int ,y) _tobe(a + 4);

 _repin(x) _as(x + 5);

 _repin(x)
   x += nip(y);
 _as(x);

 _nip(x)
 {
  _nip(y)
  {
   qDebug() << "y = " << y;
   qDebug() << "x = " << x;
  };
 };
}

void test1(pinned(int ,x))
{
 _unpin(x);
 x += 50;
 qDebug() << "x1 = " << x;
}

void demo4()
{
 int a = 9;
 _pin(int ,x) _tobe(a + 3);
 a += 20;
 test1(x_pinned);

 qDebug() << "x = " << nip(x);
}

int main(int argc, char *argv[])
{
 demo1();
 demo2();
 demo3();
 demo4();

 return 0;
}


//int main(int argc, char *argv[])
//{
// QApplication qapp(argc, argv);
// add_to_data_set("test", 1);
//// QLispConsole* main_console = QLispConsole::getInstance();
//// QLispConsole_Dialog* qld = new QLispConsole_Dialog("", main_console);
//// qld->show();
// qapp.exec();
// return 0;
//}
