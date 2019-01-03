
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "ds-kdmi/ds-kdmi-model.h"

#include "kans.h"

USING_KANS(KDMI)


int main(int argc, char **argv)
{
 KDMI_Model kdm;
 kdm.init_from_files(DATA_FOLDER "/chapters/all.g.txt", DATA_FOLDER "/chapters/all.txt");
 return 0;
}
