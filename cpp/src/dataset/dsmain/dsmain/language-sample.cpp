
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "language-sample.h"

#include "language-sample-group.h"

#include <QDebug>

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Language_Sample::Language_Sample(QString text)
  :  text_(text), index_(0), chapter_(0), page_(0), group_(nullptr)
{

}

QString Language_Sample::pre_with_mark()
{
 if(amark_.isEmpty())
   return precomment_;
 if(precomment_.isEmpty())
   return amark_;
 return QString("%1 %2").arg(amark_).arg(postcomment_);
}

QString Language_Sample::alternate_or_text()
{
 if(alternate_.isEmpty())
   return text_;
 return alternate_;
}

int Language_Sample::get_group_id()
{
 if(group_)
 {
  return group_->id();
 }
 return 0;
}

Language_Sample_Group* Language_Sample::get_ref_group()
{
 if(group_)
 {
  if(group_->ref_group())
    return group_->ref_group();
  return group_;
 }
 return nullptr;
}

QString Language_Sample::get_serialization()
{
 QString result = QString("%1 %2 %3 %4 <%5>%6\n").arg(index_)
   .arg(sub_index_).arg(chapter_).arg(page_)
   .arg(precomment_).arg(postcomment_);

 if(!ser_pre_.isEmpty())
 {
  result.prepend(ser_pre_ + "\n");
 }

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

void Language_Sample::read_samples_from_file
(QString path, QVector<Language_Sample*>& result,
 QVector<Language_Sample_Group*>& groups)
{
 QString text = load_file(path);
 QStringList qsl = text.split('\n');
 QString loc_code;
 QString alternate;
 QString speaker;
 QString amark;
 QString ser_pre;

 Language_Sample_Group* current_ref_group = nullptr;
 bool awaiting_ref_group = false;
 int ref_group_id = 0;

 int group_count = 0;
 int gid = 0;

 for(QString qs : qsl)
 {
  if(qs.isEmpty())
    continue;

  if(qs == "--")
  {
   ser_pre = qs;
   current_ref_group = nullptr;
   continue;
  }
  if(qs.startsWith('+'))
  {
   ser_pre = qs;
   if(qs == "++")
   {
    current_ref_group = nullptr;
    awaiting_ref_group = true;
   }
   else
   {
    ref_group_id = qs.mid(1).toInt();
   }
   continue;
  }

  if(qs.startsWith('#'))
  {
   gid = qs.mid(2).simplified().toInt();
   continue;
  }
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

  QString pre;
  QString post;

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

  if(!ser_pre.isEmpty())
  {
   samp->set_ser_pre(ser_pre);
   ser_pre.clear();
  }

  if(gid)
  {
   Language_Sample_Group* g = groups[gid - 1];
   samp->set_group(g);

   if(awaiting_ref_group)
   {
    current_ref_group = g;
    awaiting_ref_group = false;
   }
   else if(ref_group_id)
   {
    g->set_ref_group(groups[ref_group_id - 1]);
    ref_group_id = 0;
   }
   else if(current_ref_group)
   {
    g->set_ref_group(current_ref_group);
   }
  }
  else
  {
   QString tid = QString("%1-%2-%3")
        .arg(samp->chapter()).arg(samp->page())
        .arg(samp->index());
   qDebug() << "group missing: " << tid;
//   QString tid = QString("%1-%2-%3")
//     .arg(samp->chapter()).arg(samp->page())
//     .arg(samp->index());
//   Language_Sample_Group* g = groups.value(tid);
//   if(!g)
//   {
//    ++group_count;
//    g = new Language_Sample_Group(group_count, tid);
//    groups[tid] = g;
//    g->set_chapter(samp->chapter());
//    g->set_page(samp->page());
//   }
//   samp->set_group(g);
  }

  samp->get_ref_group()->push_back(samp);

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


  if(!samp->speaker().isEmpty())
  {
   samp->get_ref_group()->check_set_form("Dialog");
  }


  loc_code.clear();
  result.push_back(samp);
 }
}

