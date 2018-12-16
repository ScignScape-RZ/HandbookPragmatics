
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "lexpair-sxpr.h"

#include "styles.h"

#include "kans.h"

#include <QStack>
#include <QDebug>

USING_KANS(DSM)


Lexpair_Sxpr::Lexpair_Sxpr()
{
}




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


//void add_word()
//{

//}

void Lexpair_Sxpr::add_chief_node(Chief_Node* cn)
{
 chief_nodes_[cn->word] = cn;
}

void Lexpair_Sxpr::check_rewind(QString& chief)
{
 Chief_Node* cn = chief_nodes_[chief];
 ++cn->current_rewind;
 if(cn->current_rewind == cn->local_depth)
 {
  chief = cn->parent_chief;
 }
}

void Lexpair_Sxpr::read(QString qs)
{
 int lambda = 0;
 int rewind = 0;
 int lparen = 0;
 int rparen = 0;

 int local_lparen = 0;

 QString acc;

 QStack<int> lambdas;

 QMap<QPair<int, int>, Chief_Node*> cars;

 QString current_chief;

 QSet<Dock_Link> docks;

 auto add_word = [&]()
 {
  QPair<int, int> pr = {lparen, rparen};

  qDebug() << "lambda: " << lambda;

  if(lambda == 0)
  {
   qDebug() << "lambda: " << lambda;

   Chief_Node* cn = new Chief_Node {acc, current_chief, local_lparen, 0};
   add_chief_node(cn);

   cars[pr] = cn;
   local_lparen = 0;
   current_chief = acc;
  }
  else
  {
   docks.insert({current_chief, acc,
                 {lambda, rewind + 1}});
   check_rewind(current_chief);
  }
  acc.clear();
 };

 for(int i = 0; i < qs.length(); ++i)
 {
  QChar qc = qs[i];
  if(qc == '(')
  {
   lambdas.push(lambda);
   ++lparen;
   ++local_lparen;
  }
  else if(qc == ')')
  {
   add_word();
   ++rparen;
  }
  else if(qc == ' ')
  {
   if(acc.isEmpty())
   {
    // // blank space after close paren
   }
   else
   {
    add_word();
    ++lambda;
   }
  }
  else
   acc += qc;
 }
}

uint qHash(const Dock_Link &dl)
{
 return qHash(dl.to_pr_pr());
}

bool operator ==(const Dock_Link &lhs, const Dock_Link &rhs)
{
 return lhs.to_pr_pr() == rhs.to_pr_pr();
}
