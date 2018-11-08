
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "language-sample.h"

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Language_Sample::Language_Sample(QString text)
  :  text_(text), index_(0), chapter_(0), page_(0)
{

}

void Language_Sample::read_samples_from_file(QString path, QVector<Language_Sample*>& result)
{
 QString text = load_file(path);
 QStringList qsl = text.split('\n');
 QString loc_code;
 for(QString qs : qsl)
 {
  if(qs.isEmpty())
    continue;
  if(loc_code.isEmpty())
  {
   loc_code = qs.simplified();
   continue;
  }
  QStringList ls = loc_code.split(' ');
  Language_Sample* samp = new Language_Sample(qs);
  samp->set_index(ls[0].toInt());
  samp->set_sub_index(ls[1]);
  samp->set_chapter(ls[2].toInt());
  samp->set_page(ls[3].toInt());
  loc_code.clear();
  result.push_back(samp);
 }
}

