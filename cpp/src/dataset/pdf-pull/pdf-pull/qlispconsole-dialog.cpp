
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "qlispconsole-dialog.h"

#include "qlispconsole.h"


#include <QApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QScrollArea>
#include <QFileDialog>
#include <QTabWidget>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>

#include <QGraphicsRectItem>
#include <QRubberBand>

#include <QPlainTextEdit>
#include <QTextStream>

#include <QTableWidget>

#include <QMessageBox>

#include <QMenu>

#include <QHeaderView>

#include <QListWidget>


#include "styles.h"

#include "qsns.h"


QLispConsole_Dialog::QLispConsole_Dialog(QString text,
  QLispConsole* main_console, QWidget* parent)
  :  QDialog(parent), main_console_(main_console)
{

 button_box_ = new QDialogButtonBox(this);

 button_ok_ = new QPushButton("OK");
 button_proceed_ = new QPushButton("Proceed");
 button_cancel_ = new QPushButton("Cancel");

 button_ok_->setDefault(false);
 button_ok_->setAutoDefault(false);

 button_proceed_->setDefault(false);
 button_proceed_->setAutoDefault(false);
 //?button_cancel_->setDefault(true);

 button_ok_->setEnabled(false);

 button_box_->addButton(button_ok_, QDialogButtonBox::AcceptRole);
 button_box_->addButton(button_proceed_, QDialogButtonBox::ApplyRole);
 button_box_->addButton(button_cancel_, QDialogButtonBox::RejectRole);


// QString colorful_button_style_sheet = colorful_button_style_sheet_();
// QString colorful_toggle_button_style_sheet = colorful_button_quiet_style_sheet_(); //colorful_toggle_button_style_sheet_();
// QString colorful_button_style_sheet_down = colorful_button_style_sheet_down_();
// QString colorful_button_quiet_style_sheet = colorful_button_quiet_style_sheet_();
// QString tab_style_sheet = tab_style_sheet_();
// QString basic_button_style_sheet = basic_button_style_sheet_();




 button_ok_->setStyleSheet(basic_button_style_sheet_());
 button_proceed_->setStyleSheet(basic_button_style_sheet_());
 button_cancel_->setStyleSheet(basic_button_style_sheet_());

// button_ok_->setStyleSheet(button_close_light_style_sheet_());
// button_proceed_->setStyleSheet(button_close_light_style_sheet_());
// button_cancel_->setStyleSheet(button_close_light_style_sheet_());


 connect(button_proceed_, SIGNAL(clicked()), this, SLOT(proceed()));
 connect(button_box_, SIGNAL(accepted()), this, SLOT(accept()));
 connect(button_box_, SIGNAL(rejected()), this, SLOT(cancel()));


 main_layout_ = new QVBoxLayout;

 main_text_edit_ = new QTextEdit(text, this);

// main_splitter_->setOrientation(Qt::Vertical);
 main_splitter_ = new QSplitter(Qt::Vertical, this);

 main_layout_->addWidget(main_splitter_);

 main_splitter_->addWidget(main_text_edit_);

 main_notebook_ = new QTabWidget(this);
 cmd_log_text_edit_ = new QTextEdit(this);
 http_log_text_edit_ = new QTextEdit(this);

 main_notebook_->addTab(main_console_, "REPL");
 main_notebook_->addTab(cmd_log_text_edit_, "CMD Logs");
 main_notebook_->addTab(http_log_text_edit_, "HTTP Logs");

 main_splitter_->addWidget(main_notebook_);

 main_layout_->addWidget(button_box_);

 setLayout(main_layout_);

 setMinimumWidth(600);

}

QString QLispConsole_Dialog::get_current_text()
{
 return main_text_edit_->toPlainText();
}

void QLispConsole_Dialog::log_command(QStringList* list, QString* cmd)
{
 cmd_log_text_edit_->document()->setPlainText(list->join("\n"));
}


int QLispConsole_Dialog::get_current_line_number()
{
 return main_console_->history_index();
}

QLispConsole_Dialog::~QLispConsole_Dialog()
{
}


void QLispConsole_Dialog::cancel()
{
 Q_EMIT(rejected());
 Q_EMIT(canceled(this));
 Q_EMIT(rejected());
// close();
//
 close();
}

void QLispConsole_Dialog::proceed()
{
 //?Q_EMIT(proceed_requested(this));
}


void QLispConsole_Dialog::accept()
{
 Q_EMIT(accepted(this));
// close();
}
