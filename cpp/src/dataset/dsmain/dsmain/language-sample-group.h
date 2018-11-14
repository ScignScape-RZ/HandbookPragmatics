
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef LANGUAGE_SAMPLE_GROUP__H
#define LANGUAGE_SAMPLE_GROUP__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>


KANS_(DSM)

class Language_Sample;

class Language_Sample_Group : QVector<Language_Sample*>
{
 int id_;
 int chapter_;
 int page_;

 QString classification_;
 QString text_id_;

 Language_Sample_Group* ref_group_;

public:

 Language_Sample_Group(int id, QString text_id);


 ACCESSORS(int ,id)
 ACCESSORS(int ,chapter)
 ACCESSORS(int ,page)
 ACCESSORS(QString ,text_id)
 ACCESSORS(QString ,classification)
 ACCESSORS(Language_Sample_Group* ,ref_group)

 QString get_serialization();

 static void read_groups_from_file(QString path,
  QVector<Language_Sample_Group*>& result);


};


_KANS(DSM)

#endif // LANGUAGE_SAMPLE_GROUP__H
