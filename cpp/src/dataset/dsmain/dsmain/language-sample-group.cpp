
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "language-sample-group.h"

#include "language-sample.h"

#include <QDebug>

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Language_Sample_Group::Language_Sample_Group(int id, QString text_id)
  :  id_(id),
    text_id_(text_id), chapter_(0), page_(0),
    ref_group_(nullptr), rg_id_(0)
{

}

QString Language_Sample_Group::get_main_text()
{
 if(main_text_.isEmpty())
   return first_sample_text();
 return main_text_;
}

QString Language_Sample_Group::first_sample_text()
{
 if(isEmpty())
   return QString();
 return first()->text();
}

QStringList Language_Sample_Group::all_sample_text()
{
 QStringList result;
 for(Language_Sample* samp : *this)
 {
  result << samp->text();
 }
 return result;
}

QString Language_Sample_Group::get_serialization(int& rgc)
{
 QString result = QString("%1 %2 %3 %4 <%5>%6\n").arg(id_)
   .arg(rg_id_).arg(chapter_).arg(page_)
   .arg(classification_).arg(text_id_);

 if(ref_group_)
 {
  int rgi = ref_group_->id();
  if(rgi == id_)
  {
   ++rgc;
   result.prepend(QString("=%1\n").arg(rgc));
  }
  else
    result.prepend(QString("+%1\n").arg(rgi));
 }

 if(!main_text_.isEmpty())
 {
  result.prepend(QString("$ %1\n").arg(main_text_));
 }


 return result;
}

QString Language_Sample_Group::get_form()
{
 if(classification_.isEmpty())
   return "Text";
 int index = classification_.indexOf(':');
 if(index == -1)
 {
  return classification_;
 }
 return classification_.left(index);
}

bool Language_Sample_Group::match_classification(const QSet<QString>& qset)
{
 if(classification_.isEmpty())
   return qset.contains("Text");
 int index = classification_.indexOf(':');
 if(index == -1)
 {
  return qset.contains(classification_);
 }
 return qset.contains(classification_.left(index))
   && qset.contains(classification_.mid(index + 1));
}

QString Language_Sample_Group::get_issue()
{
 if(classification_.isEmpty())
   return "(N_A)";
 int index = classification_.indexOf(':');
 if(index == -1)
 {
  return "(N_A)";
 }
 return classification_.mid(index + 1);
}

void Language_Sample_Group::check_set_form(QString f)
{
 if(classification_.isEmpty())
 {
  classification_ = f;
 }
 else if(classification_.startsWith("?:"))
 {
  classification_.replace(0, 1, f);
 }
}

void Language_Sample_Group::read_groups_from_file(QString path,
 QVector<Language_Sample_Group*>& result)
{
 QString text = load_file(path);
 QStringList qsl = text.split('\n');

 int rid = 0;
 int hrid = 0;

 QString mtext;

 for(QString qs : qsl)
 {
  if(qs.isEmpty())
    continue;

  if(qs.startsWith('$'))
  {
   mtext = qs.mid(2);
   continue;
  }

  if(qs.startsWith('+'))
  {
   rid = qs.mid(1).toInt();
   continue;
  }

  if(qs.startsWith('='))
  {
   hrid = qs.mid(1).toInt();
   continue; // for now ...
  }

  QString tid;
  QString cl;

  int index = qs.indexOf('<');
  if(index != -1)
  {
   QString tc = qs.mid(index + 1);
   qs = qs.left(index).simplified();
   int i1 = tc.indexOf('>');
   if(i1 == -1)
   {
    tid = tc;
   }
   else
   {
    cl = tc.left(i1);
    tid = tc.mid(i1 + 1);
   }
  }

  QStringList ls = qs.split(' ');

  Language_Sample_Group* g = new Language_Sample_Group(ls[0].toInt(), tid);

  int rgid = ls[1].toInt();
  if(hrid)
  {
   if(rgid && (rgid != hrid))
   {
    qDebug() << QString("conflicting rgid: %1, %2").arg(hrid).arg(rgid);
   }
   g->set_rg_id(hrid);
   hrid = 0;
  }
  else
    g->set_rg_id(rgid);



  g->set_chapter(ls[2].toInt());
  g->set_page(ls[3].toInt());

  if(rid)
  {
   g->set_ref_group(result[rid - 1]);
   rid = 0;
  }

  if(!mtext.isEmpty())
  {
   g->set_main_text(mtext);
   mtext.clear();
  }
  result.push_back(g);

 }
}
