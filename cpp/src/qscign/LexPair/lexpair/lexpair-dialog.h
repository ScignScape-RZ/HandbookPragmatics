
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef LEXPAIR_DIALOG__H
#define LEXPAIR_DIALOG__H

#include <QObject>

#include <QMetaType>

#include <QList>

#include <QPoint>

#include <QDialog>
#include <QDebug>
#include <QComboBox>

#include <functional>

#include "accessors.h"
#include "qsns.h"


#include <functional>

#include "kans.h"

KANS_CLASS_DECLARE(DSM ,Lexpair_Sxpr)
USING_KANS(DSM)

class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QGridLayout;
class QTextEdit;
class QFrame;
class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QPlainTextEdit;
class QBoxLayout;
class QButtonGroup;
class QGroupBox;
class QTableWidget;
class QTableWidgetItem;
class QStackedWidget;

class QDockWidget;

class QFrame;
class QMainWindow;

class QCheckBox;


class Lexpair_Dialog : public QDialog
{
 Q_OBJECT

 QHBoxLayout* minimize_layout_;

 QDialogButtonBox* button_box_;
 QPushButton* button_ok_;
 QPushButton* button_cancel_;
 QPushButton* button_proceed_;

 QFrame* sentence_frame_;
 QHBoxLayout* sentence_layout_;

 QButtonGroup* sentence_button_group_;
 QList<QPushButton*> multi_selected_buttons_;

 //QPushButton* multi_button_;
 QPushButton* add_button_;
 QLabel* add_label_;
 QString original_add_label_text_;
 QHBoxLayout* add_layout_;

 QPushButton* reset_button_;
 QPushButton* _focus_button_;

 QGridLayout* sxpr_layout_;
 QPushButton* sxpr_mode_button_;

 QButtonGroup* sxpr_button_group_;
 QPushButton* sxpr_clear_button_;
 QPushButton* ll_paren_button_;
 QPushButton* left_paren_button_;
 QPushButton* lend_paren_button_;

 QPushButton* right_paren_button_;
 QPushButton* rr_paren_button_;
 QPushButton* sxpr_cc_button_;
 QPushButton* sxpr_read_button_;
 QPushButton* sxpr_splice_button_;
 QPushButton* sxpr_back_splice_button_;
 QPlainTextEdit* sxpr_text_edit_;

 QVBoxLayout* main_layout_;

 qint8 source_id_;
 qint8 target_id_;
 qint8 pivot_id_;

 QStringList sentence_;

 QTableWidget* pairs_table_widget_;

 struct Vertical_Header_Map_Value
 {
  QStringList words;
  int logical_index;
  QTableWidgetItem* pin_prior;
  QTableWidgetItem* pin_next;
//  QTableWidgetItem* _prior;
//  QTableWidgetItem* _next;
 };

 QMap<QTableWidgetItem*, Vertical_Header_Map_Value> vertical_header_map_;

 Lexpair_Sxpr* sxpr_;

 QMainWindow* mw_;
 QVBoxLayout* mw_layout_;

 QDockWidget* grammar_dock_widget_;
 QVBoxLayout* grammar_layout_;
 QFrame* grammar_frame_;
 QComboBox* grammar_combo_;
 QHBoxLayout* grammar_combo_layout_;
 QStackedWidget* grammar_stacked_widget_;

 QGridLayout* link_grammar_layout_;
 QFrame* link_grammar_frame_;

 QGridLayout* dependency_grammar_layout_;
 QFrame* dependency_grammar_frame_;

 QGridLayout* transform_elements_layout_;
 QFrame* transform_elements_frame_;

 QFrame* mw_frame_;
 //QVBoxLayout* main_frame_layout_;

 void check_multi_select_button(QPushButton* btn);

 void set_cell_text(int r, int c, QString text);
 QString get_cell_text(int r, int c);

 void clear_splice(bool checked = false);
 void splice_multi();

 void back_splice_multi();

 void strip_characters(QPushButton* btn);

 void plan_focus_button(QPushButton* btn);

 QString get_info_text(QString folder, QString topic, QString& details);

 void add_pair_line(QPair<QString, QString>& words, QPair<quint8, quint8>& pos);
 void reinsert_pair_line(int li, int ovi, int nvi, int dir = 0);

 int remove_pairs_row(int row);

 void reset_add();
 void force_recheck(int id);

 // //
 // ls.read("((quickly won)(The (home team)))");

 void clear_buttons();
 void enable_sxpr_buttons(bool en);

 struct Pair_Row
 {
  QTableWidgetItem* twi;
  QString left;
  QString right;
  QString mid;
 };

 struct Pair_Triple
 {
  qint8 left;
  qint8 right;
  qint8 mid;

  QPair<qint8, QPair<qint8, quint8>> to_pr_pr() const
  {
   return {left, {right, mid}};
  }
 };

 QMap<Pair_Triple, Pair_Row> pairs_;

 int pairs_count_;

 void sxpr_insert_text(QString text, qint16 at_position = 0, qint16 move_cursor_offset = 1);
 void set_button_width(QPushButton* btn, int margin = 0);
 void check_pair();

 void read_sxpr(QString qs);

 void sxpr_highlight_balanced(int i1, int i2);
 void check_sxpr_balance(QChar qch, int pos);
 void check_paren_balance(QChar qch, int pos, int max,
   std::function<void(int, QChar&)> fn, std::function<void(int, int)> cb);

public:

 Lexpair_Dialog(QStringList sent, QWidget* parent);
 ~Lexpair_Dialog();

 bool eventFilter(QObject *obj, QEvent *event);

 void lg_label_cb(QString text);
 void dg_label_cb(QString text);
 void tr_label_cb(QString text);


 void show_lg_info(QString text);
 void show_dg_info(QString text);
 void auto_insert_lg(QString text, QString pm = QString());
 void auto_insert_dg(QString text);
 void auto_insert_tr_micro(QString text);
 void auto_insert_tr_macro(QString text);
 void auto_insert_tr(int r, int c, QString text);

 static QStringList split(QString qs)
 {
  QStringList result = qs.simplified().split(' ');

//  QStringList qsl = result;
//  if(qsl.removeDuplicates() == 0)
//    return result;

  QMap<QString, QPair<quint8, quint8>> counts;

  for(QString qs: result)
  {
   ++counts[qs].first;
//   int c = result.count(qs);
//   if(c > 1)
//     counts[qs] = {c, 0};
  }

  for(QString& qs: result)
  {
   QPair<quint8, quint8>& pr = counts[qs];
   if(pr.first > 1)
   {
    ++pr.second;
    qs += QString("\\%1").arg(pr.second);
    qDebug() << qs;
   }
//   auto it = counts.find(qs);
//   if(it != counts.end())
//   {
//    QPair<quint8, quint8>& pr = *it;
//    ++pr.second;
//    qs += "\\" + pr.second;
//   }
  }

  return result;
  //return qs.simplified().split(' ');
 }

 friend bool operator< (const Pair_Triple& lhs, const Pair_Triple& rhs)
 {
  return lhs.to_pr_pr() < rhs.to_pr_pr();
 }

// friend uint qHash(const Dock_Link &dl)
// {
//  return qHash(dl.to_pr_pr());
// }

// friend operator ==(const Dock_Link &lhs, const Dock_Link &rhs)
// {
//  return lhs.to_pr_pr() == rhs.to_pr_pr();
// }

Q_SIGNALS:

 void canceled(QDialog*);
 void accepted(QDialog*);


public Q_SLOTS:

 void accept();
 void cancel();
// void proceed();

};



#endif  // LEXPAIR_DIALOG__H


