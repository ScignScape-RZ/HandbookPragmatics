
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DATASET__H
#define DATASET__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>
#include <QMap>

KANS_(DSM)

class Language_Sample;
class Language_Sample_Group;

class Dataset
{
 QString file_;
 QVector<Language_Sample*> samples_;

 QVector<int> chapter_pages_;

// QMap<QString, Language_Sample_Group*> groups_by_text_;
// typedef QMap<QString, Language_Sample_Group*> groups_by_text_type;

 QVector<Language_Sample_Group*> groups_;

 QStringList issues_;
 QStringList forms_;

public:

 Dataset(QString file);

 ACCESSORS__RGET(QVector<Language_Sample*> ,samples)
//? ACCESSORS__RGET(groups_by_text_type ,groups_by_text)
 ACCESSORS__RGET(QVector<Language_Sample_Group*> ,groups)

 ACCESSORS__RGET(QStringList ,issues)
 ACCESSORS__RGET(QStringList ,forms)

 void save_raw_file(QString text, int page, int num);
 void parse_to_samples(QString text, int page, int num);
 int get_chapter_number_from_page(int page);

 void save_to_file();
 void get_serialization(QString& text, QString& gtext);
};


_KANS(DSM)

#endif // DATASET__H
