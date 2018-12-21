
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "lexpair-dialog.h"

#include "styles.h"

#include "lexpair/lexpair-sxpr.h"

#include "textio.h"

USING_KANS(TextIO)

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

//#include <QExtraSelection>

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


void set_height(QPlainTextEdit* qpte, int rows)
{
 QFontMetrics qfm(qpte->font());
 int h = qfm.lineSpacing();
 qpte->setFixedHeight(rows * h + 10);
}

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
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->show_lg_info(text);
 });

 if(scl->property("styled_info").toBool())
 {
  qm->addAction("Unmark",
    [dlg, scl, text]()
  {
   scl->unstyle();
  });
 }

 qm->addAction("Auto Insert",
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->auto_insert_lg(text);
 });
 qm->addAction("Auto Insert (+-)",
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->auto_insert_lg(text, "+-");
 });
 qm->addAction("Auto Insert (-+)",
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->auto_insert_lg(text, "-+");
 });

 qm->popup(event->globalPos());

//
// dlg->lg_label_cb(text);
}



void _dg_info_cb(QObject* obj, QMouseEvent* event,
  ScignStage_Clickable_Label* scl, QString text)
{
 Lexpair_Dialog* dlg = qobject_cast<Lexpair_Dialog*>(obj);
 QMenu* qm = new QMenu(dlg);
 qm->addAction("Show Info",
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->show_dg_info(text);
 });

 if(scl->property("styled_info").toBool())
 {
  qm->addAction("Unmark",
    [dlg, scl, text]()
  {
   scl->unstyle();
  });
 }

 qm->addAction("Auto Insert",
   [dlg, scl, text]()
 {
  scl->unstyle();
  dlg->auto_insert_dg(text);
 });

 qm->popup(event->globalPos());

}

Lexpair_Dialog::Lexpair_Dialog(QStringList sent, QWidget* parent)
  : QDialog(parent), _focus_button_(nullptr), source_id_(0),
    target_id_(0), pivot_id_(0), pairs_count_(0), sxpr_(nullptr)
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
 sentence_frame_ = new QFrame(this);

 sentence_button_group_ = new QButtonGroup(this);

 sentence_button_group_->setExclusive(false);

 for(QString qs : sent)
 {
  QPushButton* b = new QPushButton(qs, sentence_frame_);

  b->setStyleSheet(
    "QPushButton:checked:!focus:hover{background:darkRed}\n"
    "QPushButton:checked:focus:hover{background:teal}\n"
    "QPushButton:pressed:!focus{border:1px ridge darkRed; background:white}\n"
    "QPushButton:pressed:focus:hover{border:1px ridge teal; background:white}\n"
    "*[multi_select=\"true\"]{background:orange;}\n"
   );

  set_button_width(b);
  b->setCheckable(true);
  b->setFocusPolicy(Qt::StrongFocus);
  b->setAutoDefault(false);
  b->setDefault(false);
  b->installEventFilter(this);
  sentence_button_group_->addButton(b);
  sentence_layout_->addWidget(b);
 }

 connect(sentence_button_group_,
   QOverload<int, bool>::of(&QButtonGroup::buttonToggled),
   [this](int id, bool checked)
 {
//  qDebug() << id;
//  qDebug() << checked;

  if(sxpr_mode_button_->isChecked())
  {
   QAbstractButton* btn = sentence_button_group_->button(id);
   if(btn->property("multi_select").toBool())
   {
    clear_splice();
    {
     QSignalBlocker qsb(sentence_button_group_);
     btn->setChecked(false);
    }
    return;
   }

   QString text = sentence_.at(-id-2);
   sxpr_insert_text(text + " ");
   if(!checked)
   {
    force_recheck(id);
   }
   sentence_button_group_->button(id)->setFocus();
   plan_focus_button(sxpr_cc_button_);
   return;
  }
  if(source_id_ == 0)
  {
   source_id_ = id;
   add_label_->setText(sentence_.at(-id-2));
   plan_focus_button(reset_button_);
  }
  else if(target_id_ == 0)
  {
   target_id_ = id;
   add_label_->setText(QString("%1 %2").arg(add_label_->text()).arg(sentence_.at(-id-2)));
   plan_focus_button(add_button_);
  }
  else if(pivot_id_ == 0)
  {
   pivot_id_ = id;
   add_label_->setText(QString("%1 (%2)").arg(add_label_->text()).arg(sentence_.at(-id-2)));
   plan_focus_button(add_button_);
   if(!checked)
     force_recheck(id);
  }
  else if(id == source_id_)
  {
   qint8 tid = target_id_;
   target_id_ = id;
   source_id_ = tid;
   add_label_->setText(QString("%1 %2 (%3)").arg(sentence_.at(-tid-2))
     .arg(sentence_.at(-id-2)).arg(sentence_.at(-pivot_id_-2)));
   plan_focus_button(add_button_);
   force_recheck(id);
  }
  else if(id == target_id_)
  {
   qint8 sid = source_id_;
   source_id_ = id;
   target_id_ = sid;
   add_label_->setText(QString("%1 %2 (%3)").arg(sentence_.at(-id-2))
     .arg(sentence_.at(-sid-2)).arg(sentence_.at(-pivot_id_-2)));
   plan_focus_button(add_button_);
   force_recheck(id);
  }
  else if(id == pivot_id_)
  {
   force_recheck(id);
   plan_focus_button(add_button_);
  }
  else
  {
   qint8 tid = target_id_;
   target_id_ = id;
   source_id_ = tid;
   add_label_->setText(QString("%1 %2 (%3)").arg(sentence_.at(-tid-2))
     .arg(sentence_.at(-id-2)).arg(sentence_.at(-pivot_id_-2)));
   plan_focus_button(add_button_);
   force_recheck(id);
  }

 });

 sentence_layout_->addStretch();
 sentence_frame_->setLayout(sentence_layout_);
 sentence_frame_->installEventFilter(this);
 main_layout_->addWidget(sentence_frame_);

 add_layout_ = new QHBoxLayout;
// multi_button_ = new QPushButton("Multi", this);
// multi_button_->setCheckable(true);
// multi_button_->setChecked(false);
// add_layout_->addWidget(multi_button_);

 add_button_ = new QPushButton("Add", this);
 add_label_ = new QLabel(this);
 add_label_->setText("(Pair/Triple)");
 original_add_label_text_ = "(Pair/Triple)";
 add_label_->setMaximumWidth(300);
 add_label_->setMinimumWidth(300);

 connect(add_button_, &QPushButton::clicked, [this]()
 {
  if( (source_id_ == 0 ) || (target_id_ == 0) )
  {
   reset_button_->setFocus();
  }
  else
    check_pair();
 });

 add_layout_->addWidget(add_button_);
 add_layout_->addWidget(add_label_);

 reset_button_ = new QPushButton("Reset", this);
 connect(reset_button_, &QPushButton::clicked, [this]()
 {
  reset_add();
 });

 add_layout_->addSpacing(30);
 add_layout_->addWidget(reset_button_);
 add_layout_->addStretch();



 main_layout_->addLayout(add_layout_);

 sxpr_layout_ = new QGridLayout;

 sxpr_mode_button_ = new QPushButton("SXPR\nMode", this);
 sxpr_layout_->addWidget(sxpr_mode_button_, 0, 0, 2, 1);

 sxpr_mode_button_->setCheckable(true);
 sxpr_mode_button_->setStyleSheet(colorful_toggle_button_quiet_style_sheet_());

 connect(sxpr_mode_button_, &QPushButton::clicked, [this]
 {
  reset_add();
  add_label_->setText(original_add_label_text_);
  if(sxpr_mode_button_->isChecked())
  {
   enable_sxpr_buttons(true);
   sxpr_cc_button_->setFocus();
  }
  else
    enable_sxpr_buttons(false);
 });

 sxpr_button_group_ = new QButtonGroup(this);
 sxpr_button_group_->setExclusive(false);

 sxpr_clear_button_ = new QPushButton("Clear", this);
 sxpr_clear_button_->setDefault(false);
 sxpr_clear_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_clear_button_, 0, 1);
 connect(sxpr_clear_button_, &QPushButton::clicked, [this]
 {
  sxpr_text_edit_->setPlainText("");
  clear_buttons();
 });
 sxpr_button_group_->addButton(sxpr_clear_button_);
 set_button_width(sxpr_clear_button_, 16);

 ll_paren_button_ = new QPushButton("<- (", this);
 ll_paren_button_->setDefault(false);
 ll_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(ll_paren_button_, 0, 2);
 connect(ll_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_insert_text("( ", 1, 0);
 });
 sxpr_button_group_->addButton(ll_paren_button_);
 set_button_width(ll_paren_button_, 16);

 lend_paren_button_ = new QPushButton("(", this);
 lend_paren_button_->setDefault(false);
 lend_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(lend_paren_button_, 0, 3);
 connect(lend_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_insert_text("( ");
 });
 sxpr_button_group_->addButton(lend_paren_button_);
 set_button_width(lend_paren_button_, 16);

 left_paren_button_ = new QPushButton("( ->", this);
 left_paren_button_->setDefault(false);
 left_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(left_paren_button_, 0, 4);
 connect(left_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_insert_text("( ", -1);
 });
 sxpr_button_group_->addButton(left_paren_button_);
 set_button_width(left_paren_button_, 16);

 right_paren_button_ = new QPushButton(")", this);
 right_paren_button_->setDefault(false);
 right_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(right_paren_button_, 0, 5);
 connect(right_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_insert_text(") ");
 });
 sxpr_button_group_->addButton(right_paren_button_);
 set_button_width(right_paren_button_, 16);

 rr_paren_button_ = new QPushButton(") ->", this);
 rr_paren_button_->setDefault(false);
 rr_paren_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(rr_paren_button_, 0, 6);
 connect(rr_paren_button_, &QPushButton::clicked, [this]
 {
  sxpr_insert_text(") ", -1, 0);
 });
 sxpr_button_group_->addButton(rr_paren_button_);
 set_button_width(rr_paren_button_, 16);

 sxpr_cc_button_ = new QPushButton("Copy", this);
 sxpr_cc_button_->setDefault(false);
 sxpr_cc_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_cc_button_, 0, 7);
 connect(sxpr_cc_button_, &QPushButton::clicked, [this]
 {
  QApplication::clipboard()->setText(sxpr_text_edit_->toPlainText().simplified());
 });
 sxpr_cc_button_->setFocusPolicy(Qt::StrongFocus);
 sxpr_cc_button_->setStyleSheet("QPushButton:focus{border:1px groove teal;}"
   "QPushButton:focus:hover{border:2px groove teal;}" );
 sxpr_button_group_->addButton(sxpr_cc_button_);
 set_button_width(sxpr_cc_button_, 16);

 sxpr_read_button_ = new QPushButton("Read", this);
 sxpr_read_button_->setDefault(false);
 sxpr_read_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_read_button_, 0, 8);
 connect(sxpr_read_button_, &QPushButton::clicked, [this]
 {
  read_sxpr(sxpr_text_edit_->toPlainText().simplified());
 });
 sxpr_button_group_->addButton(sxpr_read_button_);
 set_button_width(sxpr_read_button_, 16);

 sxpr_splice_button_ = new QPushButton("Splice", this);
 sxpr_splice_button_->setDefault(false);
 sxpr_splice_button_->setAutoDefault(false);
 sxpr_layout_->addWidget(sxpr_splice_button_, 0, 9);
 connect(sxpr_splice_button_, &QPushButton::clicked, [this]
 {
  read_sxpr(sxpr_text_edit_->toPlainText().simplified());
 });
 sxpr_button_group_->addButton(sxpr_splice_button_);
 set_button_width(sxpr_splice_button_, 16);

 sxpr_layout_->setColumnStretch(10, 1);

 enable_sxpr_buttons(false);

 sxpr_text_edit_ = new QPlainTextEdit(this);
 sxpr_layout_->addWidget(sxpr_text_edit_, 1, 1, 1, 10);

 set_height(sxpr_text_edit_, 1);

 connect(sxpr_text_edit_, &QPlainTextEdit::cursorPositionChanged, [this]()
 {
  //int pos =
  QTextCursor qtc = sxpr_text_edit_->textCursor();//.position();
  QChar qch;
  if(! qtc.atBlockEnd() )
    qch = sxpr_text_edit_->toPlainText().at(qtc.position());
  else qch = '\0';
    check_sxpr_balance(qch, qtc.position());
 });

 sxpr_layout_->setColumnStretch(9, 1);

 main_layout_->addLayout(sxpr_layout_);

 pairs_table_widget_ = new QTableWidget(this);

 pairs_table_widget_->setColumnCount(10);

 pairs_table_widget_->setHorizontalHeaderLabels({"", "Pivot", "lg:Source\nExpectation",
   "lg:Target\nExpectation", "lg:\nDescription",
   "ds:Source\nExpectation", "dg:Target\nExpectation",
   "dg:\nDescription",
                                        QChar(0x0002b13),//QChar(0x00027f8),
                                        QChar(0x000003BB)});

 for(int i = 1; i < 8; ++i)
   pairs_table_widget_->setColumnWidth(i, 75);

 pairs_table_widget_->setColumnWidth(8, 19);
 pairs_table_widget_->setColumnWidth(9, 16);

 main_layout_->addWidget(pairs_table_widget_);

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
  // //  Uncomment to regenerate files ...
//  QString path = QString("%1/%2.txt").arg(LINK_GRAMMAR_ABOUT_FOLDER).arg(qs);
//  save_file(path, "\n@\n");

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
  // //  Uncomment to regenerate files ...
//  QString path = QString("%1/%2.txt").arg(DEPENDENCY_GRAMMAR_ABOUT_FOLDER).arg(qs);
//  save_file(path, "\n@\n");

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

void Lexpair_Dialog::force_recheck(int id)
{
 QSignalBlocker qsb(sentence_button_group_);
 sentence_button_group_->button(id)->setChecked(true);
}

void Lexpair_Dialog::check_sxpr_balance(QChar qch, int pos)
{
 check_paren_balance(qch, pos, sxpr_text_edit_->toPlainText().size(),
   [this](int i, QChar& result)
   {
    result = sxpr_text_edit_->toPlainText().at(i);
   },
   [this](int i1, int i2)
   {
    sxpr_highlight_balanced(i1, i2);
    //qDebug() << ": " << i1 << ", " << i2;
   });
}

void Lexpair_Dialog::plan_focus_button(QPushButton* btn)
{
 _focus_button_ = btn;
}

void Lexpair_Dialog::clear_splice()
{
 for(QPushButton* btn: multi_selected_buttons_)
 {
  btn->setProperty("multi_select", false);
  btn->style()->unpolish(btn);
  btn->style()->polish(btn);
 }
 sxpr_splice_button_->setEnabled(false);
}

bool Lexpair_Dialog::eventFilter(QObject* obj, QEvent* event)
{
 if(QFrame* fr = qobject_cast<QFrame*>(obj))
 {
  if(event->type() == QEvent::MouseMove)
  {
   const QMouseEvent* const qme = static_cast<const QMouseEvent*>( event );

   if(qme->buttons() & Qt::RightButton)//multi_button_->isChecked())
   {
    if(!sxpr_mode_button_->isChecked())
    {
     return true;
    }
    if(QWidget* qw = fr->childAt(qme->pos()))
    {
     if(QPushButton* btn = qobject_cast<QPushButton*>(qw))
     {
      multi_selected_buttons_.push_back(btn);
      sxpr_splice_button_->setEnabled(true);
      btn->setProperty("multi_select", true);
      btn->style()->unpolish(btn);
      btn->style()->polish(btn);
     }
    }
   }
  }
 }
 if(QPushButton* btn = qobject_cast<QPushButton*>(obj))
 {
  if(event->type() == QEvent::Leave)
  {
//   const QMouseEvent* const qme = static_cast<const QMouseEvent*>( event );

   if(_focus_button_)
     _focus_button_->setFocus();
   if(btn->isChecked())
   {
    btn->setFocusPolicy(Qt::NoFocus);
   }
   return true;
  }
 }
 return QDialog::eventFilter(obj, event);
}

void Lexpair_Dialog::sxpr_insert_text(QString text, qint16 at_position,
  qint16 move_cursor_offset)
{
 QTextCursor qtc = sxpr_text_edit_->textCursor();

 if(at_position > 0)
 {
  qtc.movePosition(QTextCursor::Start);
  qtc.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, at_position - 1);
 }
 else if(at_position < 0)
 {
  qtc.movePosition(QTextCursor::End);
  qtc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor, -1 - at_position);
 }

 qtc.insertText(text);

 if(move_cursor_offset == 1)
 {
  sxpr_text_edit_->setTextCursor(qtc);
 }
 else if(move_cursor_offset > 1)
 {
  sxpr_text_edit_->setTextCursor(qtc);
  qtc.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, move_cursor_offset - 1);
 }
 else if(move_cursor_offset < 0)
 {
  sxpr_text_edit_->setTextCursor(qtc);
  qtc.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor, -move_cursor_offset);
 }

 sxpr_text_edit_->setFocus();
}

void Lexpair_Dialog::sxpr_highlight_balanced(int i1, int i2)
{
 int m = qMin(i1, i2);
 int x = qMax(i1, i2);

 QTextCursor mc = QTextCursor(sxpr_text_edit_->document());
 mc.setPosition(m);
 mc.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

 QTextCursor xc = QTextCursor(sxpr_text_edit_->document());
 xc.setPosition(x);
 xc.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

 QTextEdit::ExtraSelection esm;
 esm.cursor = mc;
 if(m == i2)
 {
  esm.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
  esm.format.setUnderlineColor(Qt::magenta);
 }
 esm.format.setForeground(Qt::darkMagenta);
 esm.format.setFontWeight(QFont::Bold);

 QTextEdit::ExtraSelection esx;
 esx.cursor = xc;
 if(x == i2)
 {
  esx.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
  esx.format.setUnderlineColor(Qt::green);
 }
 esx.format.setForeground(Qt::darkGreen);
 esm.format.setFontWeight(QFont::Bold);


 sxpr_text_edit_->setExtraSelections({esm, esx});

}

void Lexpair_Dialog::reset_add()
{
 source_id_ = 0;
 target_id_ = 0;
 pivot_id_ = 0;
 clear_buttons();
 reset_button_->setFocus();
}

void Lexpair_Dialog::check_paren_balance(QChar qch, int pos, int max,
  std::function<void(int, QChar&)> fn, std::function<void(int, int)> cb)
{
 static QMap<QChar, QPair<QChar, qint8>> static_map {{
  { '(', {')', +1} },
  { ')', {'(', -1} },
   }};

 QPair<QChar, qint8> pr = static_map.value(qch);
 int dir = pr.second;
 if(dir == 0)
   return;

 QChar current;
 int count = 0;
 int p = pos + dir;
 while((p >= 0) && (p < max))
 {
  fn(p, current);
  if(current == pr.first)
  {
   if(count == 0)
   {
    cb(pos, p);
    break;
   }
   --count;
  }
  else if(current == qch)
    ++count;
  p += dir;
 }
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

 //pairs_[{source_id_, target_id_, id}]  = {twi, sl, sr};
 pairs_table_widget_->setRowCount(pairs_count_ + 1);
 pairs_table_widget_->setItem(pairs_count_, 0, twi);

 QTableWidgetItem* twi_md = new QTableWidgetItem(QChar(0x2b05));
 pairs_table_widget_->setItem(pairs_count_, 1, twi_md);

 QTableWidgetItem* twi_rw = new QTableWidgetItem(QString::number(pos.first));
 pairs_table_widget_->setItem(pairs_count_, 8, twi_rw);

 QTableWidgetItem* twi_lm = new QTableWidgetItem(QString::number(pos.second));
 pairs_table_widget_->setItem(pairs_count_, 9, twi_lm);

 ++pairs_count_;
}

void Lexpair_Dialog::check_pair()
{
 auto it = pairs_.find({source_id_, target_id_, pivot_id_});
 qint8 found_mid = 0;

 QTableWidgetItem* twi = nullptr;
 QString sl;
 QString sr;
 QString sm;

 if(it == pairs_.end())
 {
  if(pivot_id_ == 0)
  {
   for(Pair_Triple pt: pairs_.keys())
   {
    if( (pt.left == source_id_) && (pt.right == target_id_) )
    {
     twi = pairs_[pt].twi;
     sl = sentence_.at(-source_id_-2);
     sr = sentence_.at(-target_id_-2);
     sm = sentence_.at(-pt.mid-2);
    }
   }
  }
  if(!twi)
  {
   sl = sentence_.at(-source_id_-2);
   sr = sentence_.at(-target_id_-2);
   twi = new QTableWidgetItem(QString(
     "%1 %2").arg(sl).arg(sr));

   pairs_[{source_id_, target_id_, pivot_id_}]  = {twi, sl, sr};
   pairs_table_widget_->setRowCount(pairs_count_ + 1);
   pairs_table_widget_->setItem(pairs_count_, 0, twi);

   if(pivot_id_)
   {
    sm = sentence_.at(-pivot_id_-2);
    QTableWidgetItem* mtwi = new QTableWidgetItem(sm);
    pairs_table_widget_->setItem(pairs_count_, 1, mtwi);
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
 pairs_table_widget_->selectRow(twi->row());
 source_id_ = 0;
 target_id_ = 0;
 clear_buttons();
}

void Lexpair_Dialog::enable_sxpr_buttons(bool en)
{
 QSignalBlocker qsb(sentence_button_group_);
 for(QAbstractButton* b : sxpr_button_group_->buttons())
 {
  b->setEnabled(en);
 }
 sxpr_splice_button_->setEnabled(false);
}

void Lexpair_Dialog::clear_buttons()
{
 QSignalBlocker qsb(sentence_button_group_);
 for(QAbstractButton* b : sentence_button_group_->buttons())
 {
  b->setChecked(false);
  b->setFocusPolicy(Qt::StrongFocus);
 }
}

void Lexpair_Dialog::set_button_width(QPushButton* button, int margin)
{
 QSize size = button->fontMetrics().size(Qt::TextShowMnemonic, button->text());
 size += {margin, 0};
 QStyleOptionButton opt;
 opt.initFrom(button);
 opt.rect.setSize(size);
 button->setMaximumSize(
   button->style()->sizeFromContents(QStyle::CT_PushButton,
   &opt, size, button));
}


void Lexpair_Dialog::lg_label_cb(QString text)
{
 QModelIndex qmi = pairs_table_widget_->currentIndex();
 if(qmi.isValid())
 {
  if(qmi.column() > 0)
    set_cell_text(qmi.row() + 1, qmi.column() + 1, text);
 }
}

void Lexpair_Dialog::dg_label_cb(QString text)
{
 QModelIndex qmi = pairs_table_widget_->currentIndex();
 if(qmi.isValid())
 {
  if(qmi.column() > 0)
    set_cell_text(qmi.row() + 1, qmi.column() + 1, text);
 }
}


void Lexpair_Dialog::set_cell_text(int r, int c, QString text)
{
 QTableWidgetItem* twi = pairs_table_widget_->item(r - 1, c - 1);
 if(!twi)
 {
  twi = new QTableWidgetItem;
  pairs_table_widget_->setItem(r - 1, c - 1, twi);
 }
 twi->setText(text);
}


QHBoxLayout* add_minimize_frame(QMessageBox* qmb, std::function<void()> fn)
{
 for(QAbstractButton* qab: qmb->buttons())
 {
  if (qmb->buttonRole(qab) == QMessageBox::ActionRole)
  {
   qab->click();
   break;
  }
 }

 QPushButton* minimize_button = new QPushButton(QChar(0x2735), qmb);
 QLabel* minimize_label = new QLabel(qmb);
 QHBoxLayout* result = new QHBoxLayout;
 minimize_label->setText("Minimize");
 minimize_button->setStyleSheet(colorful_small_button_style_sheet_());

 QApplication::connect(minimize_button, &QPushButton::clicked, fn);

 QFrame* fr = new QFrame;

 fr->setFrameStyle(QFrame::Sunken);
 fr->setFrameShape(QFrame::Panel);

 QHBoxLayout* fr_layout = new QHBoxLayout;
 fr_layout->addWidget(minimize_button);
 fr_layout->addWidget(minimize_label);

 fr->setLayout(fr_layout);

 result->addWidget(fr);
 result->addStretch();

 if(QGridLayout* gl = dynamic_cast<QGridLayout*>(qmb->layout()))
 {
  gl->addLayout(result, gl->rowCount(), 0, 1, gl->columnCount());
 }
 else
 {
  //qmb->layout()->addLayout(result);
 }

 return result;
}


void Lexpair_Dialog::show_lg_info(QString text)
{
 QString dt;
 QString summary = get_info_text(LINK_GRAMMAR_ABOUT_FOLDER, text, dt);

 QMessageBox* qmb = new QMessageBox;
 qmb->setAttribute(Qt::WA_DeleteOnClose);
 qmb->setText(summary);
 qmb->setIcon(QMessageBox::Information);
 qmb->setWindowTitle(QString("Link: %1").arg(text));
 qmb->setDetailedText(dt);
 qmb->addButton("Ok", QMessageBox::YesRole);
 add_minimize_frame(qmb, [qmb]()
 {
  qmb->setWindowState(Qt::WindowMinimized);
 });
 qmb->setModal(false);
 qmb->open(this, "");
}

QString Lexpair_Dialog::get_info_text(QString folder, QString topic, QString& details)
{
 QString path = QString("%1/%2.txt").arg(folder).arg(topic);
 QString text = load_file(path);
 int index = text.indexOf("\n@\n");
 if(index != -1)
 {
  details = text.mid(index + 3);
  return text.left(index);
 }
 details = text;
 return topic;
}

void Lexpair_Dialog::show_dg_info(QString text)
{
 QString dt;

 // // because "aux.txt" doesn't work ...
 if(text == "aux")
   text += "_";

 QString summary = get_info_text(DEPENDENCY_GRAMMAR_ABOUT_FOLDER, text, dt);

 QMessageBox* qmb = new QMessageBox;
 qmb->setAttribute(Qt::WA_DeleteOnClose);
 qmb->setText(summary);
 qmb->setIcon(QMessageBox::Information);
 qmb->setWindowTitle(QString("Dependency: %1").arg(text));
 qmb->setDetailedText(dt);
 qmb->addButton("Ok", QMessageBox::YesRole);
 add_minimize_frame(qmb, [qmb]()
 {
  qmb->setWindowState(Qt::WindowMinimized);
 });
 qmb->setModal(false);
 qmb->open(this, "");
}

void Lexpair_Dialog::auto_insert_dg(QString text)
{
 int cr = pairs_table_widget_->currentRow();
 set_cell_text(cr + 1, 8, text);
}

void Lexpair_Dialog::auto_insert_lg(QString text, QString pm)
{
 int cr = pairs_table_widget_->currentRow();
 if(cr != -1)
 {
  set_cell_text(cr + 1, 5, text);
  if(pm == "+-")
  {
   set_cell_text(cr + 1, 3, text + "+");
   set_cell_text(cr + 1, 4, text + "-");
  }
  else if(pm == "-+")
  {
   set_cell_text(cr + 1, 3, text + "-");
   set_cell_text(cr + 1, 4, text + "+");
  }
 }
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
