
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "language-sample-group.h"

#include <QDebug>

#include "textio.h"


USING_KANS(DSM)
USING_KANS(TextIO)

Language_Sample_Group::Language_Sample_Group(int id, QString text_id)
  :  id_(id),
    text_id_(text_id), chapter_(0), page_(0)
{

}


QString Language_Sample_Group::get_serialization()
{
 QString result = QString("%1 %2 %3 <%4>%5\n").arg(id_)
   .arg(chapter_).arg(page_)
   .arg(classification_).arg(text_id_);

 return result;
}
