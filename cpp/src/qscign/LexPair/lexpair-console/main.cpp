
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "lexpair/lexpair-dialog.h"

#include <QApplication>
#include <QDebug>

#include "kans.h"

#include "textio.h"

USING_KANS(TextIO)

#include <QThread>


//USING_KANS(DSM)


int main(int argc, char **argv)
{
 QApplication qapp(argc, argv);


 Lexpair_Dialog dlg({"The", "white", "dog"}, nullptr);


 dlg.show();

 return qapp.exec();
}
