
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "dsmain/dataset.h"

#include "qlispconsole-dialog.h"
#include "qlispconsole.h"

USING_KANS(DSM)


enum class Callbacks { N_A, Save, Parse, Database_Save };

Callbacks parse_callback(const QString& cmd)
{
 static QMap<QString, Callbacks> static_map {{
  {"sv", Callbacks::Save},
  {"pr", Callbacks::Parse},
  {"ds", Callbacks::Database_Save},
 }};

 return static_map.value(cmd, Callbacks::N_A);
}

void run_dataset_callback(Dataset* ds, QString text, const QString& cmd, int* i, QString& qs)
{
 int page = 0;
 int num = 0;
 QRegularExpression prx("@(\\d+)");
 QRegularExpression nrx("\#(\\d+)");
 QRegularExpressionMatch prxm;
 QRegularExpressionMatch nrxm;
 if(text.indexOf(prx, 0, &prxm) != -1)
 {
  page = prxm.captured(1).toInt();
 }
 if(text.indexOf(nrx, 0, &nrxm) != -1)
 {
  num = nrxm.captured(1).toInt();
 }

 Callbacks cb = parse_callback(cmd.toLower());
 switch (cb)
 {
 case Callbacks::Save:
  {
   ds->save_raw_file(text, page, num);
  }
  break;
 case Callbacks::Parse:
  {
   ds->parse_to_samples(text, page, num);
  }
  break;
 case Callbacks::Database_Save:
  {
   ds->save_to_file();
  }
  break;
 default:
  break;
 }
}

void add_to_data_set(QString qs, int page)
{
 static Dataset* ds = nullptr;
 if(!ds)
 {
  ds = new Dataset(DEFAULT_TEMP_DATASET_FILE);
 }

 QString text = QString("@%1 # %2").arg(page).arg(qs);
 QLispConsole* main_console = QLispConsole::getInstance();

 QLispConsole_Dialog* qld = new QLispConsole_Dialog(text, main_console);
 main_console->set_cb([ds, qld](const QString& cmd, int* i, QString& qs)
 {
  run_dataset_callback(ds, qld->get_current_text(), cmd, i, qs);
 });

 qld->show();
}
