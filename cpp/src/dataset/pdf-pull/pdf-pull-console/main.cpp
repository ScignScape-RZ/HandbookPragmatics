
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "pdf-pull/qlispconsole-dialog.h"
#include "pdf-pull/qlispconsole.h"

#include "pdf-pull/pdf-pull.h"

#include <QApplication>

#include <QVector>


int main(int argc, char *argv[])
{
 QApplication qapp(argc, argv);
 add_to_data_set("test", 1);
 qapp.exec();
 return 0;
}

// QLispConsole* main_console = QLispConsole::getInstance();
// QLispConsole_Dialog* qld = new QLispConsole_Dialog("", main_console);
// qld->show();
