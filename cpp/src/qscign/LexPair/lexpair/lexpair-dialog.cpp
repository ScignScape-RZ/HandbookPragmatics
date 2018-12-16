
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "lexpair-dialog.h"

#include "styles.h"




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

#include <QtMultimedia/QMediaPlayer>

#include <QPainter>
#include <QPushButton>
#include <QLabel>

#include <QMessageBox>
#include <QDebug>

#include <QHeaderView>



Lexpair_Dialog::Lexpair_Dialog(QStringList sent, QWidget* parent)
  : QDialog(parent), left_id_(0), right_id_(0), pairs_count_(0)
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
  if(left_id_ == id)
  {
   left_id_ = 0;
   right_id_ = 0;
  }
  else if(left_id_ == 0)
  {
   left_id_ = id;
  }
  else if(right_id_ == 0)
  {
   right_id_ = id;
  }
  else
  {
   check_pair(id);
  }
 });

 sentence_layout_->addStretch();

 main_layout_->addLayout(sentence_layout_);

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

 pair_list_->setColumnCount(6);

 pair_list_->setHorizontalHeaderLabels({"", "Left Expectation",
   "Right Expectation", "Link Description", "Rewind", "Lambda"});

 main_layout_->addWidget(pair_list_);

 main_layout_->addWidget(button_box_);

 setLayout(main_layout_);
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


}

void Lexpair_Dialog::check_pair(qint8 id)
{
 auto it = pairs_.find({left_id_, right_id_, id});
 if(it == pairs_.end())
 {
  QString sl = sentence_.at(-left_id_-2);
  QString sr = sentence_.at(-right_id_-2);
  QString sm = sentence_.at(-id-2);

  //pairs_[{left_id_, right_id_}]  = {nullptr, sl, sr};
  //qDebug() << QStringList{sl, sr};

  QTableWidgetItem* twi = new QTableWidgetItem(QString(
    "%1 %2 (%3)").arg(sl).arg(sr).arg(sm));

  pairs_[{left_id_, right_id_, id}]  = {twi, sl, sr};
  pair_list_->setRowCount(pairs_count_ + 1);
  pair_list_->setItem(pairs_count_, 0, twi);
  ++pairs_count_;
 }
 else
 {
  QTableWidgetItem* twi = (*it).twi;
  QString sl = (*it).left;
  QString sr = (*it).right;
  qDebug() << QStringList{sl, sr};
  //twi->setSelected(true);
  //twi->row();
  pair_list_->selectRow(twi->row());
  //.sethighlighted();
  //twi->setCheckState(Qt::Checked);
 }
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
