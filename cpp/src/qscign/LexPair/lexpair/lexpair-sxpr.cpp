
//          Copyright Nathaniel Christen 2019.
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



//void add_word()
//{

//}

void Lexpair_Sxpr::add_dock(Chief_Node* cn, QString word)
{
 int &lc = cn->lamba_counts.last();
 ++lc;
 docks_.insert({cn->word, word, {lc, cn->lamba_counts.size()}});
}

void Lexpair_Sxpr::add_chief_node(Chief_Node* cn)
{
 chief_nodes_[cn->word] = cn;
 cn->lamba_counts.push_back(0);
}

int Lexpair_Sxpr::reset_lambda(Chief_Node* cn)
{
 return 0;
}

void Lexpair_Sxpr::check_rewind(QString& chief)
{
 Chief_Node* cn = chief_nodes_[chief];

 if(cn->lamba_counts.size() == cn->local_depth)
 {
  chief = cn->parent_chief;
 }
 else
 {
  cn->lamba_counts.push_back(0);
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

 //auto add_tail


 for(int i = 0; i < qs.length(); ++i)
 {
  QChar qc = qs[i];
  if(qc == '(')
  {
   //lambdas.push(lambda);
   lambda = 0;
   ++lparen;
   ++local_lparen;
  }
  else if(qc == ')')
  {
   if(!acc.isEmpty())
   {
    add_dock(current_chief, acc); //, lambda, rewind + 1}});
    acc.clear();
    check_rewind(current_chief);
   }
   if(!current_chief.isEmpty())
     lambda = reset_lambda(current_chief);
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
    QPair<int, int> pr = {lparen, rparen};

    if(lambda == 0)
    {
     Chief_Node* cn = new Chief_Node {acc, current_chief, local_lparen};
     add_chief_node(cn);

     cars[pr] = cn;
     local_lparen = 0;
     if(!current_chief.isEmpty())
       add_dock(current_chief, acc);
     current_chief = acc;
    }
    else
    {
     add_dock(current_chief, acc); //, lambda, rewind + 1}});
     check_rewind(current_chief);
    }
    acc.clear();
    ++lambda;
   }
  }
  else
   acc += qc;
 }
}


