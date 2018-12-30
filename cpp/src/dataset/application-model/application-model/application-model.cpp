
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "application-model.h"

#include "ScignStage-ling/ScignStage-ling-dialog.h"

#include "kauvir-phaon/kph-command-package.h"


USING_KANS(Phaon)


#include <QDebug>


Application_Model::Application_Model(ScignStage_Ling_Dialog* ling_dialog)
  :  ling_dialog_(ling_dialog)
{

}

void Application_Model::expand_sample(ScignStage_Ling_Dialog* dlg, int index)
{
 dlg->expand_sample(index - 1);
}

void Application_Model::launch_lexpair_dialog(ScignStage_Ling_Dialog* dlg, QString s)
{
 dlg->launch_lexpair_dialog(s);
}

Application_Model::~Application_Model() {}
