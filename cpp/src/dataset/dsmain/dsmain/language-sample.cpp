
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

QString Language_Sample::get_serialization()
{
 QString result = QString("%1 %2 %3 %4 <%5>%6\n").arg(index_)
   .arg(sub_index_).arg(chapter_).arg(page_)
   .arg(precomment_).arg(postcomment_);
 if(!alternate_.isEmpty())
 {
  result += QString("/ %1\n").arg(alternate_);
 }
 result += text_;
 return result;
}

void Language_Sample::read_samples_from_file(QString path, QVector<Language_Sample*>& result)
{
 QString text = load_file(path);
 QStringList qsl = text.split('\n');
 QString loc_code;
 QString alternate;
 for(QString qs : qsl)
 {
  QString pre;
  QString post;
  if(qs.isEmpty())
    continue;
  if(qs.startsWith('/'))
  {
   alternate = qs.mid(2).simplified();
   continue;
  }
  if(loc_code.isEmpty())
  {
   loc_code = qs.simplified();
   continue;
  }
  int index = loc_code.indexOf('<');
  if(index != -1)
  {
   QString pp = loc_code.mid(index + 1);
   loc_code = loc_code.left(index).simplified();
   int i1 = pp.indexOf('>');
   if(i1 == -1)
   {
    pre = pp;
   }
   else
   {
    pre = pp.left(i1);
    post = pp.mid(i1 + 1);
   }
  }
  QStringList ls = loc_code.split(' ');
  Language_Sample* samp = new Language_Sample(qs);
  samp->set_index(ls[0].toInt());
  samp->set_sub_index(ls[1]);
  samp->set_chapter(ls[2].toInt());
  samp->set_page(ls[3].toInt());
  samp->set_precomment(pre);
  samp->set_postcomment(post);
  if(!alternate.isEmpty())
  {
   samp->set_alternate(alternate);
   alternate.clear();
  }
  loc_code.clear();
  result.push_back(samp);
 }
}

