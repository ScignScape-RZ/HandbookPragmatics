
//          Copyright Nathaniel Christen 2018.
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

#include <functional>

#include "accessors.h"
#include "qsns.h"


#include <functional>

#include "kans.h"


//USING_KANS(DSM)

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

class QCheckBox;


class Lexpair_Dialog : public QDialog
{
 Q_OBJECT

 QDialogButtonBox* button_box_;
 QPushButton* button_ok_;
 QPushButton* button_cancel_;
 QPushButton* button_proceed_;

 QHBoxLayout* sentence_layout_;

 QButtonGroup* sentence_button_group_;

 QGridLayout* sxpr_layout_;
 QPushButton* sxpr_mode_button_;
 QPushButton* sxpr_clear_button_;
 QPushButton* left_paren_button_;
 QPushButton* right_paren_button_;
 QPushButton* sxpr_read_button_;
 QLineEdit* sxpr_line_edit_;

 QVBoxLayout* main_layout_;

 qint8 left_id_;
 qint8 right_id_;

 QStringList sentence_;

 QTableWidget* pair_list_;

 void clear_buttons();

 struct Pair_Row
 {
  QTableWidgetItem* twi;
  QString left;
  QString right;
  QString mid;
 };

 struct Pair_Triple
 {
  quint8 left;
  quint8 right;
  quint8 mid;

  QPair<quint8, QPair<quint8, quint8>> to_pr_pr() const
  {
   return {left, {right, mid}};
  }
 };

 QMap<Pair_Triple, Pair_Row> pairs_;

 int pairs_count_;

 void set_button_width(QPushButton* btn);
 void check_pair(qint8 id);


public:

 Lexpair_Dialog(QStringList sent, QWidget* parent);
 ~Lexpair_Dialog();

 static QStringList split(QString qs)
 {
  return qs.simplified().split(' ');
 }

 friend operator< (const Pair_Triple& lhs, const Pair_Triple& rhs)
 {
  return lhs.to_pr_pr() < rhs.to_pr_pr();
 }

Q_SIGNALS:

 void canceled(QDialog*);
 void accepted(QDialog*);


public Q_SLOTS:

 void accept();
 void cancel();
// void proceed();

};



#endif  // LEXPAIR_DIALOG__H


