
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DS_KDMI_MODEL__H
#define DS_KDMI_MODEL__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>
#include <QMap>


KANS_CLASS_DECLARE(DSM, Language_Sample)
KANS_CLASS_DECLARE(DSM, Language_Sample_Group)


USING_KANS(DSM)


KANS_(KDMI)


class KDMI_Model
{
 QVector<Language_Sample*> samples_;
 QVector<Language_Sample_Group*> groups_;

public:

 KDMI_Model();

 ACCESSORS__RGET(QVector<Language_Sample*> ,samples)
 ACCESSORS__RGET(QVector<Language_Sample_Group*> ,groups)


 void init_from_files(QString s, QString g);


};


_KANS(DSM)

#endif // DS_KDMI_MODEL__H
