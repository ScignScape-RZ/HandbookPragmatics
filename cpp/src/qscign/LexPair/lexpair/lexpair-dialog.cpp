
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
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>

#include <QTreeWidget>

#include <QDirIterator>

#include <QPlainTextEdit>
#include <QTextStream>

#include <QtMultimedia/QMediaPlayer>

#include <QPainter>
#include <QPushButton>
#include <QLabel>

#include <QMessageBox>
#include <QDebug>

#include <QHeaderView>



Lexpair_Dialog::Lexpair_Dialog(QStringList sent, QWidget* parent)
  : QDialog(parent)
{
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

 for(QString qs : sent)
 {
  QPushButton* b = new QPushButton(qs, this);
  set_button_width(b);
  b->setCheckable(true);
  sentence_button_group_->addButton(b);
  sentence_layout_->addWidget(b);
 }

 sentence_layout_->addStretch();

 main_layout_->addLayout(sentence_layout_);
 main_layout_->addWidget(button_box_);

 setLayout(main_layout_);
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
