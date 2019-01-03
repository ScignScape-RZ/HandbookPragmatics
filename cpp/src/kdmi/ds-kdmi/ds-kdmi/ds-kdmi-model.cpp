
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "ds-kdmi-model.h"

#include "dsmain/language-sample.h"

#include "dsmain/language-sample-group.h"

#include "textio.h"

#include <QDateTime>


USING_KANS(KDMI)
USING_KANS(DSM)
USING_KANS(TextIO)

KDMI_Model::KDMI_Model()
{

}


void KDMI_Model::init_from_files(QString s, QString g)
{
 Language_Sample_Group::read_groups_from_file
   (g, groups_);

 Language_Sample::read_samples_from_file
   (s, samples_, groups_);
}

