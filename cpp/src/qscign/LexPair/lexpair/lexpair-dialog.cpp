
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "lexpair-dialog.h"

#include "styles.h"

#include "lexpair/lexpair-sxpr.h"

#include <QApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QButtonGroup>
#include <QScrollArea>
#include <QFileDialog>
#include <QTabWidget>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QClipboard>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>

#include <QTableWidget>

#include <QDirIterator>

#include <QPlainTextEdit>
#include <QTextStream>
#include <QStack>

#include <QStackedWidget>

#include <QtMultimedia/QMediaPlayer>

#include <QMenu>
#include <QPushButton>
#include <QLabel>

#include <QToolButton>

#include <QMessageBox>
#include <QDebug>

#include <QHeaderView>

#include <QHBoxLayout>

#include <QPushButton>
#include <QDockWidget>

#include <QFrame>
#include <QMainWindow>

#include "add-minimize-frame.h"

#include "ScignStage-ling/subwindows/scignstage-clickable-label.h"


void _lg_label_cb(QObject* obj, QString text)
{
 Lexpair_Dialog* dlg = qobject_cast<Lexpair_Dialog*>(obj);
 dlg->lg_label_cb(text);
}

void _dg_label_cb(QObject* obj, QString text)
{
 Lexpair_Dialog* dlg = qobject_cast<Lexpair_Dialog*>(obj);
 dlg->dg_label_cb(text);
}

void _lg_info_cb(QObject* obj, QMouseEvent* event,
  ScignStage_Clickable_Label* scl, QString text)
{
 Lexpair_Dialog* dlg = qobject_cast<Lexpair_Dialog*>(obj);
 QMenu* qm = new QMenu(dlg);
 qm->addAction("Show Info",
   [dlg, scl]()
 {
  scl->unstyle();
 });
 qm->popup(event->globalPos());

//
// dlg->lg_label_cb(text);
}

void _dg_info_cb(QObject* obj, QMouseEvent* event,
  ScignStage_Clickable_Label* scl, QString text)
{
// Lexpair_Dialog* dlg = qobject_cast<Lexpair_Dialog*>(obj);
// dlg->dg_label_cb(text);
}

Lexpair_Dialog::Lexpair_Dialog(QStringList sent, QWidget* parent)
  : QDialog(parent), left_id_(0),
    right_id_(0), medium_id_(0), pairs_count_(0), sxpr_(nullptr)
{
 sentence_ = sent;

 button_box_ = new QDialogButtonBox(this);

 button_ok_ = new QPushButton("OK");
 button_proceed_ = new QPushButton("Proceed");
 button_cancel_ = new QPushButton("Cancel");

 button_ok_->setDefault(false);
 button_ok_->setAutoDefault(false);
 button_ok_->setEnabled(false);

 button_proceed_->setDefault(false);
 button_proceed_->setAutoDefault(false);
 button_proceed_->setEnabled(true);

 button_cancel_->setDefault(true);

 button_box_->addButton(button_ok_, QDialogButtonBox::AcceptRole);
 button_box_->addButton(button_proceed_, QDialogButtonBox::ApplyRole);
 button_box_->addButton(button_cancel_, QDialogButtonBox::RejectRole);

 button_ok_->setStyleSheet(basic_button_style_sheet_());
 button_proceed_->setStyleSheet(basic_button_style_sheet_());
 button_cancel_->setStyleSheet(basic_button_style_sheet_());


 connect(button_proceed_, SIGNAL(clicked()), this, SLOT(proceed()));
 connect(button_box_, SIGNAL(accepted()), this, SLOT(accept()));
 connect(button_box_, SIGNAL(rejected()), this, SLOT(cancel()));

 main_layout_ = new QVBoxLayout;

 sentence_layout_ = new QHBoxLayout;

 sentence_button_group_ = new QButtonGroup(this);

 sentence_button_group_->setExclusive(false);

 for(QString qs : sent)
 {
  QPushButton* b = new QPushButton(qs, this);
  set_button_width(b);
  b->setCheckable(true);
  //b->setFocusPolicy(Qt::);
  b->setAutoDefault(false);
  b->setDefault(false);
  sentence_button_group_->addButton(b);
  sentence_layout_->addWidget(b);
 }

 connect(sentence_button_group_,
   QOverload<int, bool>::of(&QButtonGroup::buttonToggled),
   [this](int id, bool checked)
 {
  qDebug() << id;
  qDebug() << checked;

  if(sxpr_mode_button_->isChecked())
  {
   QString text = sentence_.at(-id-2);
   sxpr_line_edit_->setText(sxpr_line_edit_->text() + text + " ");
   return;
  }
//  if(left_id_ == id)
//  {
//   left_id_ = 0;
//   right_id_ = 0;
//  }
  if(left_id_ == 0)
  {
   left_id_ = id;
   add_label_->setText(sentence_.at(-id-2));
  }
  else if(right_id_ == 0)
  {
   right_id_ = id;
   add_label_->setText(QString("%1 %2").arg(add_label_->text()).arg(sentence_.at(-id-2)));
  }
  else if(medium_id_ == 0)
  {
   medium_id_ = id;
   add_label_->setText(QString("%1 (%2)").arg(add_label_->text()).arg(sentence_.at(-id-2)));
  }
  else
  {
   left_id_ = 0;
   right_id_ = 0;
   medium_id_ = 0;
   add_label_->setText("");
  }
 });

 sentence_layout_->addStretch();

 main_layout_->addLayout(sentence_layout_);

 add_layout_ = new QHBoxLayout;
 add_button_ = new QPushButton("Add", this);
 add_label_ = new QLabel(this);
 add_label_->setText("(Pair/Triple)");

 connect(add_button_, &QPushButton::clicked, [this]()
 {
  check_pair();
 });

 add_layout_->addWidget(add_button_);
 add_layout_->addWidget(add_label_);
 add_layout_->addStretch();
 main_layout_->addLayout(add_layout_);

 sxpr_layout_ = new QGridLayout;

 sxpr_mode_button_ = new QPushButton("SXPR\nMode", this);
 sxpr_layout_->addWidget(sxpr_mode_button_, 0, 0, 2, 1);

 sxpr_mode_button_->setCheckable(true);
 sxpr_mode_button_->setStyleSheet(colorful_toggle_button_quiet_style_sheet_());

 sxpr_clear_button_ = new QPushButton("Clear", this);
 sxpr_clear_button_->setDefault(false);
 sxpr_clear_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_clear_button_, 0, 1);
 connect(sxpr_clear_button_, &QPushButton::clicked, [this]
 {
  sxpr_line_edit_->setText("");
  clear_buttons();
 });

 ll_paren_button_ = new QPushButton("<- (", this);
 ll_paren_button_->setDefault(false);
 ll_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(ll_paren_button_, 0, 2);
 connect(ll_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_line_edit_->setText(sxpr_line_edit_->text().prepend("( "));
 });

 left_paren_button_ = new QPushButton("(", this);
 left_paren_button_->setDefault(false);
 left_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(left_paren_button_, 0, 3);
 connect(left_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_line_edit_->setText(sxpr_line_edit_->text() + "( ");
 });

 lend_paren_button_ = new QPushButton("( ->", this);
 lend_paren_button_->setDefault(false);
 lend_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(lend_paren_button_, 0, 4);
 connect(lend_paren_button_, &QPushButton::clicked, [this]
 {
  QString qs = sxpr_line_edit_->text();
  qs.replace(sxpr_line_edit_->cursorPosition(), 0, "( ");
  sxpr_line_edit_->setText(qs);
 });

 right_paren_button_ = new QPushButton(")", this);
 right_paren_button_->setDefault(false);
 right_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(right_paren_button_, 0, 5);
 connect(right_paren_button_, &QPushButton::clicked, [this]
 {
  QString qs = sxpr_line_edit_->text();
  qs.replace(sxpr_line_edit_->cursorPosition(), 0, ") ");
  sxpr_line_edit_->setText(qs);
 });

 rr_paren_button_ = new QPushButton(") ->", this);
 rr_paren_button_->setDefault(false);
 rr_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(rr_paren_button_, 0, 6);
 connect(rr_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_line_edit_->setText(sxpr_line_edit_->text() + ") ");
 });

 sxpr_cc_button_ = new QPushButton("Copy", this);
 sxpr_cc_button_->setDefault(false);
 sxpr_cc_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_cc_button_, 0, 7);
 connect(sxpr_cc_button_, &QPushButton::clicked, [this]
 {
  QApplication::clipboard()->setText(sxpr_line_edit_->text().simplified());
 });

 sxpr_read_button_ = new QPushButton("Read", this);
 sxpr_read_button_->setDefault(false);
 sxpr_read_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_read_button_, 0, 8);
 connect(sxpr_read_button_, &QPushButton::clicked, [this]
 {
  read_sxpr(sxpr_line_edit_->text().simplified());
 });

 sxpr_line_edit_ = new QLineEdit(this);
 sxpr_layout_->addWidget(sxpr_line_edit_, 1, 1, 1, 8);

 sxpr_layout_->setColumnStretch(9, 1);

 main_layout_->addLayout(sxpr_layout_);

 pair_list_ = new QTableWidget(this);

 pair_list_->setColumnCount(7);

 pair_list_->setHorizontalHeaderLabels({"", "Medium", "Left Expectation",
   "Right Expectation", "Link Description", QChar(0x0002b13),//QChar(0x00027f8),
                                        QChar(0x000003BB)});

 pair_list_->setColumnWidth(5, 19);
 pair_list_->setColumnWidth(6, 16);

 main_layout_->addWidget(pair_list_);

 mw_ = new QMainWindow;

// mw->addDockWidget(Qt::LeftDockWidgetArea, qdw);


 grammar_dock_widget_ = new QDockWidget;
 grammar_frame_ = new QFrame;
 grammar_combo_ = new QComboBox(this);
 grammar_layout_ = new QVBoxLayout;

 grammar_combo_->addItem("Link Grammar");
 grammar_combo_->addItem("Dependency Grammar");

 grammar_combo_layout_ = new QHBoxLayout;
 grammar_combo_layout_->addWidget(grammar_combo_);
 grammar_combo_layout_->addStretch();

 grammar_layout_->addLayout(grammar_combo_layout_);

 grammar_stacked_widget_ = new QStackedWidget(this);

 link_grammar_frame_ = new QFrame;
 link_grammar_layout_ = new QGridLayout;

 QStringList lg_links
 {
  "A"  "AA", "AF", "AJ", "AL", "AM", "AN", "AZ", "B", "BI",
  "BT", "BW", "C", "CC", "CO", "CP", "CQ", "CV", "CX", "D",
  "DD", "DG", "DP", "DT", "E", "EA", "EB", "EC", "EE", "EF",
  "EI", "EL", "EN", "EP", "EQ", "ER", "EW", "EZ", "FL", "FM",
  "G", "GN", "H", "HA", "I", "ID", "IN", "IV", "J", "JG", "J",
  "Q", "JT", "K", "L", "LE", "LI", "M", "MF", "MG", "MJ", "MV",
  "MX", "N", "NA", "ND", "NF", "NI", "NJ", "NM", "NN", "NO",
  "NR", "NS", "NT", "NW", "O", "OD", "OF", "ON", "OT",
  "OX", "P", "PF", "PH", "PP", "Q", "QI", "QJ", "QU", "R",
  "RJ", "RS", "RW", "S", "SF", "SFI", "SI", "SJ", "SX",
  "SXI", "TA", "TD", "TH", "TI", "TM", "TO", "TQ", "TR",
  "TS", "TT", "TW", "TY", "TZ", "U", "UN", "V", "VC", "VJ",
  "W", "WN", "WR", "WV", "X", "XJ", "Y", "YP", "YS", "Z", "ZZZ" };

 int i = 0;
 for(QString qs : lg_links)
 {
//  QToolButton* b = new QToolButton(this);
//  b->setText(qs);

  ScignStage_Clickable_Label* scl = new ScignStage_Clickable_Label(this);
  scl->setText(qs);
  scl->set_text_data(qs);
  scl->setAlignment(Qt::AlignCenter);
  scl->set_cb({&_lg_label_cb, &_lg_info_cb});

  scl->setStyleSheet("*[styled_info=\"true\"] {"
    "background-color:yellow;}");

  link_grammar_layout_->addWidget(scl, i/10, i%10);
  ++i;
  //grammar_dock_layout_->addWidget(new QPushButton(qs));
 }
 link_grammar_layout_->setRowStretch(((i-1)/10)+1, 1);

 link_grammar_frame_->setLayout(link_grammar_layout_);
 grammar_stacked_widget_->addWidget(link_grammar_frame_);

 dependency_grammar_frame_ = new QFrame;
 dependency_grammar_layout_ = new QGridLayout;

 int dg_max_col = 4;

 QStringList dg_links
 {
"acl", //clausal modifier of noun (adjectival clause)
"advcl", //adverbial clause modifier
"advmod", //adverbial modifier
"amod", //adjectival modifier
"appos", //appositional modifier
"aux", //auxiliary
"case", //case marking
"cc", //coordinating conjunction
"ccomp", //clausal complement
"clf", //classifier
"compound", //compound
"conj", //conjunct
"cop", //copula
"csubj", //clausal subject
"dep", //unspecified dependency
"det", //determiner
"discourse", //discourse element
"dislocated", //dislocated elements
"expl", //expletive
"fixed", //fixed multiword expression
"flat", //flat multiword expression
"goeswith", //goes with
"iobj", //indirect object
"list", //list
"mark", //marker
"nmod", //nominal modifier
"nsubj", //nominal subject
"nummod", //numeric modifier
"obj", //object
"obl", //oblique nominal
"orphan", //orphan
"parataxis", //parataxis
"punct", //punctuation
"reparandum", //overridden disfluency
"root", //root
"vocative", //vocative
"xcomp", //open clausal complement
 };

 int j = 0;
 for(QString qs : dg_links)
 {
  ScignStage_Clickable_Label* scl = new ScignStage_Clickable_Label(this);
  scl->setText(qs);
  scl->set_text_data(qs);
  scl->set_cb({&_dg_label_cb, &_dg_info_cb});
  scl->setAlignment(Qt::AlignLeft);
  dependency_grammar_layout_->addWidget(scl, j/dg_max_col, j%dg_max_col);
  ++j;
 }
 dependency_grammar_layout_->setRowStretch(((j-1)/dg_max_col)+1, 1);

 dependency_grammar_frame_->setLayout(dependency_grammar_layout_);
 grammar_stacked_widget_->addWidget(dependency_grammar_frame_);

 grammar_layout_->addWidget(grammar_stacked_widget_);

 connect(grammar_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
   [this](int index)
 {
  grammar_stacked_widget_->setCurrentIndex(index);
 });

 //grammar_dock_layout_->addStretch();

 grammar_frame_->setStyleSheet(
    "ScignStage_Clickable_Label:hover{"
      "background-color:white;border:1px ridge rgb(130, 40, 0);}"
//    "ScignStage_Clickable_Label:*[styled_info=\"true\"] {"
//      "background-color:yellow;}" //border:1px ridge rgb(130, 40, 200);}"rgb(130, 240, 200)
//    "ScignStage_Clickable_Label*[styled_info=\"false\"] {"
//      "background-color:red;}" //border:1px ridge rgb(130, 40, 200);}"rgb(130, 240, */200)
//    "ScignStage_Clickable_Label {"
//      "background-color:blue;}" //border:1px ridge rgb(130, 40, 200);}"rgb(130, 240, 200)
    );

 grammar_frame_->setLayout(grammar_layout_);
 grammar_dock_widget_->setWidget(grammar_frame_);
 grammar_dock_widget_->setAllowedAreas(Qt::AllDockWidgetAreas);

 mw_->addDockWidget(Qt::RightDockWidgetArea, grammar_dock_widget_);

 mw_layout_ = new QVBoxLayout;

 mw_frame_ = new QFrame(this);
 mw_frame_->setLayout(main_layout_);
 mw_->setCentralWidget(mw_frame_);

 minimize_layout_ = add_minimize_frame(button_box_, [this]()
 {
  this->setWindowState(Qt::WindowMinimized);
 });
 mw_layout_->addWidget(mw_);
 mw_layout_->addLayout(minimize_layout_);

 setLayout(mw_layout_);
 // //  mw->show();

// //main_layout_->addWidget(mw);

// //main_frame_layout_ = new QVBoxLayout;

// main_frame_ = new QFrame(this);
// main_frame_->setLayout(main_layout_);
// mw_->setCentralWidget(main_frame_);

// mw_layout_->addWidget(mw_);
// setLayout(mw_layout_);





 //setLayout(main_layout_);
}

struct Word_Node
{
 QString parent_chief;
 int local_depth;
};

void Lexpair_Dialog::read_sxpr(QString qs)
{
 qs.replace("( ", "(");
 qs.replace(" )", ")");
 qs.replace(") (", ")(");
 qDebug() << qs;

 if(sxpr_)
   delete sxpr_;

 sxpr_ = new Lexpair_Sxpr;
 sxpr_->read(qs);

 QSet<QPair<QPair<QString, QString>, QPair<quint8, quint8>>> prs;
 sxpr_->get_dock_pairs(prs);

 for(auto a: prs)
 {
  add_pair_line(a.first, a.second);
 }
}

void Lexpair_Dialog::add_pair_line(QPair<QString, QString>& words,
  QPair<quint8, quint8>& pos)
{
 QTableWidgetItem* twi = new QTableWidgetItem(QString(
   "%1 %2").arg(words.first).arg(words.second));

 //pairs_[{left_id_, right_id_, id}]  = {twi, sl, sr};
 pair_list_->setRowCount(pairs_count_ + 1);
 pair_list_->setItem(pairs_count_, 0, twi);

 QTableWidgetItem* twi_md = new QTableWidgetItem(QChar(0x2b05));
 pair_list_->setItem(pairs_count_, 1, twi_md);

 QTableWidgetItem* twi_rw = new QTableWidgetItem(QString::number(pos.first));
 pair_list_->setItem(pairs_count_, 5, twi_rw);

 QTableWidgetItem* twi_lm = new QTableWidgetItem(QString::number(pos.second));
 pair_list_->setItem(pairs_count_, 6, twi_lm);

 ++pairs_count_;

}

void Lexpair_Dialog::check_pair()
{
 auto it = pairs_.find({left_id_, right_id_, medium_id_});
 qint8 found_mid = 0;

 QTableWidgetItem* twi = nullptr;
 QString sl;
 QString sr;
 QString sm;

 if(it == pairs_.end())
 {
  if(medium_id_ == 0)
  {
   for(Pair_Triple pt: pairs_.keys())
   {
    if( (pt.left == left_id_) && (pt.right == right_id_) )
    {
     twi = pairs_[pt].twi;
     sl = sentence_.at(-left_id_-2);
     sr = sentence_.at(-right_id_-2);
     sm = sentence_.at(-pt.mid-2);
    }
   }
  }
  if(!twi)
  {
   sl = sentence_.at(-left_id_-2);
   sr = sentence_.at(-right_id_-2);
   twi = new QTableWidgetItem(QString(
     "%1 %2").arg(sl).arg(sr));

   pairs_[{left_id_, right_id_, medium_id_}]  = {twi, sl, sr};
   pair_list_->setRowCount(pairs_count_ + 1);
   pair_list_->setItem(pairs_count_, 0, twi);

   if(medium_id_)
   {
    sm = sentence_.at(-medium_id_-2);
    QTableWidgetItem* mtwi = new QTableWidgetItem(sm);
    pair_list_->setItem(pairs_count_, 1, mtwi);
   }

   ++pairs_count_;
   goto cleanup;
  }
 }
 else
 {
  twi = (*it).twi;
  sl = (*it).left;
  sr = (*it).right;
 }
cleanup:
 pair_list_->selectRow(twi->row());
 left_id_ = 0;
 right_id_ = 0;
 clear_buttons();
}

void Lexpair_Dialog::clear_buttons()
{
 QSignalBlocker qsb(sentence_button_group_);
 for(QAbstractButton* b : sentence_button_group_->buttons())
 {
  b->setChecked(false);
 }
}

void Lexpair_Dialog::set_button_width(QPushButton* button)
{
 auto textSize = button->fontMetrics().size(Qt::TextShowMnemonic, button->text());
 QStyleOptionButton opt;
 opt.initFrom(button);
 opt.rect.setSize(textSize);
 button->setMaximumSize(
 button->style()->sizeFromContents(QStyle::CT_PushButton,
   &opt, textSize, button));
}


void Lexpair_Dialog::lg_label_cb(QString text)
{
 QModelIndex qmi = pair_list_->currentIndex();
 if(qmi.isValid())
 {
  if(qmi.column() > 0)
    set_cell_text(qmi.row() + 1, qmi.column() + 1, text);
 }
}

void Lexpair_Dialog::dg_label_cb(QString text)
{
 QModelIndex qmi = pair_list_->currentIndex();
 if(qmi.isValid())
 {
  if(qmi.column() > 0)
    set_cell_text(qmi.row() + 1, qmi.column() + 1, text);
 }
}


void Lexpair_Dialog::set_cell_text(int r, int c, QString text)
{
 QTableWidgetItem* twi = pair_list_->item(r - 1, c - 1);
 if(!twi)
 {
  twi = new QTableWidgetItem;
  pair_list_->setItem(r - 1, c - 1, twi);
 }
 twi->setText(text);
}


Lexpair_Dialog::~Lexpair_Dialog()
{

}

void Lexpair_Dialog::cancel()
{
 Q_EMIT(rejected());
 Q_EMIT(canceled(this));
 close();
}

void Lexpair_Dialog::accept()
{
 Q_EMIT(accepted(this));
 close();
}
