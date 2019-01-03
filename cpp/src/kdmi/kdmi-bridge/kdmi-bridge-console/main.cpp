
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "kdmi-bridge/demo.h"
#include "../ds-kdmi/ds-kdmi/ds-kdmi-model.h"
#include "../dsmain/dsmain/language-sample-group.h"
#include "../dsmain/dsmain/language-sample.h"

#include "kans.h"

USING_KANS(KDMI)
USING_KANS(DSM)

int main(int argc, char **argv)
{
 KDMI_Model kdm;
 kdm.init_from_files(DATA_FOLDER "/chapters/all.txt", DATA_FOLDER "/chapters/all.g.txt");

 Demo demo;

 for(Language_Sample_Group* group : kdm.groups())
 {
  QStringList* qsl = new QStringList;
  for(Language_Sample* samp : *group)
  {
   qsl->append(samp->text());
  }
  demo.groups()->append(qsl);
 }

 return 0;
}
