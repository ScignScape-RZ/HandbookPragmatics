
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>



#include "qlispconsole-dialog.h"
#include "qlispconsole.h"


void add_to_data_set(QString qs, int page)
{
 QString text = QString("@%1 # %2").arg(page).arg(qs);
 QLispConsole* main_console = QLispConsole::getInstance();
 QLispConsole_Dialog* qld = new QLispConsole_Dialog(text, main_console);
 qld->show();
}
