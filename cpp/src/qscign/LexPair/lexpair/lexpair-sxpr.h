
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEXPAIR_SXPR__H
#define LEXPAIR_SXPR__H

#include <QObject>

#include <QMetaType>

#include <QList>

#include <QPoint>

#include <QDialog>

#include <functional>

#include "accessors.h"
#include "qsns.h"


#include <functional>

#include "kans.h"


KANS_(DSM)


class Lexpair_Sxpr
{
 struct Chief_Node
 {
  QString word;
  QString parent_chief;
  int local_depth;
  int current_rewind;
 };

 QMap<QString, Chief_Node*> chief_nodes_;

 void add_chief_node(Chief_Node* cn);

 void check_rewind(QString& chief);

public:

 Lexpair_Sxpr();

 void read(QString qs);

};

_KANS(DSM)

#endif  // LEXPAIR_SXP__H


