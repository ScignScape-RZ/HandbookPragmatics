
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "language-sample.h"

#include <QDebug>

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Language_Sample::Language_Sample(QString text)
  :  text_(text), index_(0), chapter_(0), page_(0), group_(nullptr)
{

}

int Language_Sample::get_group_id()
{
 if(group_)
 {
  return 0;
 }
 return 0;
}

QString Language_Sample::get_serialization()
{
 QString result = QString("%1 %2 %3 %4 <%5>%6\n").arg(index_)
   .arg(sub_index_).arg(chapter_).arg(page_)
   .arg(precomment_).arg(postcomment_);

 int gid = get_group_id();
 if(gid)
 {
  result += QString("# %1\n").arg(gid);
 }

 if(!speaker_.isEmpty())
 {
  qDebug() << "speaker " << speaker_;
  result += QString(": %1\n").arg(speaker_);
 }

 if(!alternate_.isEmpty())
 {
  result += QString("/ %1\n").arg(alternate_);
 }

 if(!amark_.isEmpty())
 {
  result += QString("? %1\n").arg(amark_);
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
 QString speaker;
 QString amark;

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
  if(qs.startsWith(':'))
  {
   speaker = qs.mid(2).simplified();
   continue;
  }
  if(qs.startsWith('?'))
  {
   amark = qs.mid(2).simplified();
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

//  // // temp...
//  if(pre.startsWith('A'))
//  {
//   speaker = "A";
//   pre = pre.mid(1).simplified();
//  }
//  else if(pre.startsWith('B'))
//  {
//   speaker = "B";
//   pre = pre.mid(1).simplified();
//  }
//  else if(pre.startsWith("I1"))
//  {
//   speaker = "I1";
//   pre = pre.mid(2).simplified();
//  }
//  else if(pre.startsWith("I2"))
//  {
//   speaker = "I2";
//   pre = pre.mid(2).simplified();
//  }

//  qDebug() << loc_code;

//    // // temp...
//  if(pre.startsWith("??/*"))
//  {
//   amark = "??/*";
//   pre = pre.mid(4).simplified();
//  }
//   else if(pre.startsWith("??"))
//    {
//     amark = "??";
//     pre = pre.mid(2).simplified();
//    }
//    else if(pre.startsWith("?*"))
//    {
//     amark = "?*";
//     pre = pre.mid(2).simplified();
//    }
//    else if(pre.startsWith("*?"))
//    {
//     amark = "*?";
//     pre = pre.mid(2).simplified();
//    }
//    else if(pre.startsWith('?'))
//    {
//     amark = "?";
//     pre = pre.mid(1).simplified();
//    }
//    else if(pre.startsWith("#"))
//    {
//     amark = "#";
//     pre = pre.mid(1).simplified();
//    }
//    else if(pre.startsWith("*"))
//    {
//     amark = "*";
//     pre = pre.mid(1).simplified();
//    }

//    if(pre.startsWith('X'))
//    {
//     speaker = "X";
//     pre = pre.mid(1).simplified();
//    }
//    else if(pre.startsWith('Y'))
//    {
//     speaker = "Y";
//     pre = pre.mid(1).simplified();
//    }


  //qDebug() << "lc: " << loc_code;
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
  if(!speaker.isEmpty())
  {
   samp->set_speaker(speaker);
   speaker.clear();
  }
  if(!amark.isEmpty())
  {
   samp->set_amark(amark);
   amark.clear();
  }
  loc_code.clear();
  result.push_back(samp);
 }
}

