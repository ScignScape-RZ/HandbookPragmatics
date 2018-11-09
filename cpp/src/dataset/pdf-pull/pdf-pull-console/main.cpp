
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "pdf-pull/qlispconsole-dialog.h"
#include "pdf-pull/qlispconsole.h"

#include <QApplication>


int main(int argc, char *argv[])
{
 QApplication qapp(argc, argv);
 QLispConsole* main_console = QLispConsole::getInstance();
 QLispConsole_Dialog* qld = new QLispConsole_Dialog("", main_console);
 qld->show();
 qapp.exec();
 return 0;
}
