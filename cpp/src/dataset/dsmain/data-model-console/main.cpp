
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>

#include "dsmain/language-sample.h"

#include "dsmain/language-sample-group.h"

#include "dsmain/dataset.h"

#include "kans.h"

USING_KANS(DSM)

int main(int argc, char* argv[])
{
 Dataset ds (DATA_FOLDER "/chapters/all.txt");
 QVector<Language_Sample*>& samps = ds.samples();

// for(int i = 1; i <= 32; ++i)
// {
//  qDebug() << i;
//  QString path = QString(DATA_FOLDER "/chapters/ch%1.txt").arg(i);
//  Language_Sample::read_samples_from_file(path, samps);
// }

// QMap<QString, Language_Sample_Group*>& groups = ds.groups();

 QVector<Language_Sample_Group*>& groups = ds.groups();
 Language_Sample_Group::read_groups_from_file
   (DATA_FOLDER "/chapters/all.g.txt", groups);


 Language_Sample::read_samples_from_file
   (DATA_FOLDER "/chapters/all.txt", samps, groups);

 ds.save_to_file();




 return 0;
}
