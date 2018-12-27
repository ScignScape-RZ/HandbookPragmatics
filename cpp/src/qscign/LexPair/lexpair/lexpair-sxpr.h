
//          Copyright Nathaniel Christen 2019.
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
  //int current_rewind;

  QVector<int> lamba_counts;
 };



 struct Dock_Link
 {
  QString left;
  QString right;
  QPair<quint8, quint8> rwl;

  QPair<QPair<QString, QString>, QPair<quint8, quint8>> to_pr_pr() const
  {
   return { {left, right}, rwl };
  }
 };

 friend uint qHash(const Dock_Link &dl)
 {
  return qHash(dl.to_pr_pr());
 }

 friend bool operator ==(const Dock_Link &lhs, const Dock_Link &rhs)
 {
  return lhs.to_pr_pr() == rhs.to_pr_pr();
 }

 QMap<QString, Chief_Node*> chief_nodes_;

 QSet<Dock_Link> docks_;

 void add_chief_node(Chief_Node* cn);

 void check_rewind(QString& chief);

 int reset_lambda(QString chief)
 {
  return reset_lambda(chief_nodes_[chief]);
 }

 int reset_lambda(Chief_Node* cn);


 void add_dock(QString chief, QString word)
 {
  add_dock(chief_nodes_[chief], word);
 }

 void add_dock(Chief_Node* cn, QString word);

public:

 Lexpair_Sxpr();

 void read(QString qs);

 void get_dock_pairs(QSet<QPair<QPair<QString, QString>, QPair<quint8, quint8>>>& result)
 {
  for(auto& a : docks_)
  {
   result.insert(a.to_pr_pr());
  }
 }

};

_KANS(DSM)

#endif  // LEXPAIR_SXP__H


