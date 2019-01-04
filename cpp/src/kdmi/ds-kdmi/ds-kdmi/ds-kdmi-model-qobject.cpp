
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "ds-kdmi-model-qobject.h"

#include "ds-kdmi-model.h"

#include "kans.h"

USING_KANS(KDMI)


KDMI_Model_QObject::KDMI_Model_QObject(KDMI_Model* kdmi_model)
  :  kdmi_model_(kdmi_model)
{

}

void KDMI_Model_QObject::init_model_from_files(QString s, QString g)
{
 kdmi_model_->init_from_files(s, g);
}

