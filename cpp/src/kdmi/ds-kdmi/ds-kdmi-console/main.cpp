
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "ds-kdmi/ds-kdmi-model.h"
#include "ds-kdmi/ds-kdmi-model-qobject.h"

#include "kans.h"

USING_KANS(KDMI)

#include <QDebug>

#include "phaong.h"

typedef phaong_galaxy<QString> pgs_t;

int main(int argc, char **argv)
{
 phaong<pgs_t> pgs;

 QVector<phaong<pgs_t>::Hypernode*> hypernodes;
 QVector<phaong<pgs_t>::Hyperedge*> hyperedges;

 QPair<QVector<phaong<pgs_t>::Hypernode*>&,
   QVector<phaong<pgs_t>::Hyperedge*>&> graph {hypernodes, hyperedges};

 typedef QPair<QVector<phaong<pgs_t>::Hypernode*>&,
   QVector<phaong<pgs_t>::Hyperedge*>&> g_t;

 pgs.set_user_data(&graph);

 pgs.set_node_add_function([](phaong<pgs_t>& _pgs, phaong<pgs_t>::Hypernode* hn)
 {
  QVector<phaong<pgs_t>::Hypernode*>& hh = _pgs.user_data_as<g_t>()->first;
  hh.push_back(hn);
 });

 pgs.set_edge_add_function([](phaong<pgs_t>& _pgs, phaong<pgs_t>::Hyperedge* he)
 {
  QVector<phaong<pgs_t>::Hyperedge*>& hv = _pgs.user_data_as<g_t>()->second;
  hv.push_back(he);
 });

 phaong<pgs_t>::Hypernode* hn = pgs.new_hypernode(-8);
 pgs.set_data(hn, 13, "xx", "QString");
 pgs.get_data(hn, 13, [](QString& qs)
 {
  qDebug() << qs;
 });

 phaong<pgs_t>::Hypernode* hn1 = pgs.new_hypernode(8);
 pgs.set_data(hn1, 11, "yy", "QString");
 pgs.get_data(hn1, 3, [](QString& qs)
 {
  qDebug() << qs;
 });

 pgs.new_hyperedge(hn, hn1);

 return 0;
}

//int main(int argc, char **argv)
//{
// KDMI_Model kdm;
// // // To demonstrate, initialize the kdmi via a QObject wrapper
//  //   kdm.init_from_files(DATA_FOLDER "/chapters/all.txt", DATA_FOLDER "/chapters/all.g.txt");

// KDMI_Model_QObject kdq(&kdm);
// kdq.init_model_from_files(DATA_FOLDER "/chapters/all.txt", DATA_FOLDER "/chapters/all.g.txt");

// return 0;
//}
