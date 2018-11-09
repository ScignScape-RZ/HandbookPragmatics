
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DATASET__H
#define DATASET__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>


KANS_(DSM)


class Dataset
{
 QString file_;

public:

 Dataset(QString file);

 void save_raw_file(QString text, int page, int num);

 void parse_to_samples(QString text, int page, int num);
};


_KANS(DSM)

#endif // DATASET__H
