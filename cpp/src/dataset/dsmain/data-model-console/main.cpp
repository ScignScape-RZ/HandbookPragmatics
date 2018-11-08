
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>

#include "dsmain/language-sample.h"

#include "kans.h"

USING_KANS(DSM)

int main(int argc, char* argv[])
{
 QVector<Language_Sample*> samps;

 Language_Sample::read_samples_from_file(DATA_FOLDER "/data.txt", samps);

 return 0;
}
