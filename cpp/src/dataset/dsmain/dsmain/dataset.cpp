
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dataset.h"

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Dataset::Dataset(QString file)
  :  file_(file)
{

}

void Dataset::save_raw_file(QString text, int page, int num)
{
 QString path = QString("%1.%2.%3.txt").arg(file_)
   .arg(page).arg(num);
 save_file(path, text);
}

void Dataset::parse_to_samples(QString text, int page, int num)
{
 int current_page = page;
 int current_index = index;
 QStringList qsl = text.split("\n");
 for(QString qs : qsl)
 {
  if(qs.isEmpty())
    continue;
  if(qs.startsWith('@'))
  {
   current_page = qs.mid(1).toInt();
   continue;
  }
  if(qs.startsWith('#'))
  {
   current_index = qs.mid(1).toInt();
   continue;
  }

 }
}
