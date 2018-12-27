
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dataset.h"

#include "language-sample.h"

#include "language-sample-group.h"

#include "textio.h"

#include <QDateTime>


USING_KANS(DSM)
USING_KANS(TextIO)

Dataset::Dataset(QString file)
 :  file_(file), chapter_pages_{
     26, 52, 76, 97, 120, 145, 176,
     198, 220, 244, 264, 289, 311,
     338, 367, 388, 406, 430, 450,
     465, 486, 511, 538, 561, 585,
     601, 630, 656, 680, 698, 724,
                  747, 765 }
{
 forms_ = QStringList{{"Text", "Dialog", "Intonation", "Paragraph"}};
 issues_ = QStringList{{"Ambiguity", "Context", "Logic",
            "Scope", "Polarity", "Belief",
            "Convention", "Idioms"}};
}

int Dataset::get_chapter_number_from_page(int page)
{
 int result = 0;
 for(int i : chapter_pages_)
 {
  if(page < i)
    return result;
  ++result;
 }
 return 0;
}

void Dataset::save_raw_file(QString text, int page, int num)
{
 QString dt = QDateTime::currentDateTime().toString("dd-MM-yy--hh-mm");
 QString path = QString("%1.%2.%3.%4.txt").arg(file_)
   .arg(page).arg(num).arg(dt);
 save_file(path, text);
}

void Dataset::get_serialization(QString& text, QString& gtext)
{
 for(Language_Sample* samp : samples_)
 {
  text += samp->get_serialization() + "\n";
 }

 int rgc = 0;
 for(Language_Sample_Group* g : groups_)
 {
  gtext += g->get_serialization(rgc);
 }


// QMap<int, Language_Sample_Group*> gm;
// int sz = groups_.size();
// QMapIterator<QString, Language_Sample_Group*> it(groups_);
// while(it.hasNext())
// {
//  it.next();
//  Language_Sample_Group* g = it.value();
//  int id = g->id();
//  gm[id] = g;
// }

// for(int i = 0; i < sz; ++i)
// {
//  Language_Sample_Group* g = gm[i + 1];
//  gtext += g->get_serialization();
// }


}

void Dataset::save_to_file()
{
 QString text;
 QString gtext;
 get_serialization(text, gtext);
 QString dt = QDateTime::currentDateTime().toString("dd-MM-yy--hh-mm");
 QString path = QString("%1.%2.txt").arg(file_).arg(dt);
 save_file(path, text);

 QString gpath = QString("%1.%2.g.txt").arg(file_).arg(dt);
 save_file(gpath, gtext);

}

void Dataset::parse_to_samples(QString text, int page, int num)
{
 int current_page = page;
 int current_index = num;
 QString current_sub_index;
 QString held_precomment;
 QString held_postcomment;
 QString held_alternate;
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
  if(qs.startsWith("< "))
  {
   held_precomment = qs.mid(2).simplified();
   continue;
  }
  if(qs.startsWith("> "))
  {
   held_postcomment = qs.mid(2).simplified();
   continue;
  }
  if(qs.startsWith("/ "))
  {
   held_alternate = qs.mid(2).simplified();
   continue;
  }
  if(qs.size() < 3)
    continue;
  if( (qs[1] == '.') || (qs[1] == ')') )
  {
   current_sub_index = qs[0];
   qs = qs.mid(2);
  }
  else if( (qs[2] == '.') || (qs[2] == ')') )
  {
   current_sub_index = qs.left(2);
   qs = qs.mid(3);
  }
  else if( (qs[3] == '.') || (qs[3] == ')') )
  {
   current_sub_index = qs.left(3);
   qs = qs.mid(4);
  }
  Language_Sample* samp = new Language_Sample(qs.simplified());
  samp->set_page(current_page);
  samp->set_index(current_index);
  samp->set_sub_index(current_sub_index);
  samp->set_chapter(get_chapter_number_from_page(current_page));

  samp->set_precomment(held_precomment);
  samp->set_postcomment(held_postcomment);

  if(!held_alternate.isEmpty())
  {
   samp->set_alternate(held_alternate);
   held_alternate.clear();
  }

  held_precomment.clear();
  held_postcomment.clear();

  samples_.push_back(samp);
 }
}
