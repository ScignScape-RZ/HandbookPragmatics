
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef QLISPCONSOLE_DIALOG__H
#define QLISPCONSOLE_DIALOG__H


#include <QString>
#include <QMainWindow>

#include <QTextEdit>
//?#include <QWebView>

#include <QLabel>
#include <QPushButton>

#include <QDialog>

#include <QApplication>

#include <QComboBox>

#include "accessors.h"

#include "flags.h"

#include "qsns.h"

#include <QEvent>
#include <QMouseEvent>

#include <QTabWidget>

#include <QFrame>

//#include "incident-form-dialog.h"

class QPushButton;
class QTextEdit;
class QPlainTextEdit;
class QLineEdit;
class QTabWidget;
class QDialogButtonBox;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QFormLayout;
class QSplitter;
class QGridLayout;
class QListWidget;
class QTableWidget;
class QScrollArea;
class QGroupBox;
class QTableWidgetItem;
class QGraphicsRectItem;
class QRubberBand;

class QLispConsole;


class QLispConsole_Dialog : public QDialog
{
 Q_OBJECT

 QTextEdit* main_text_edit_;
 QSplitter* main_splitter_;

 QTabWidget* main_notebook_;
 QLispConsole* main_console_;
 QTextEdit* cmd_log_text_edit_;
 QTextEdit* http_log_text_edit_;

 QDialogButtonBox* button_box_;
 QPushButton* button_ok_;
 QPushButton* button_cancel_;
 QPushButton* button_proceed_;

 QVBoxLayout* main_layout_;

public:

 QLispConsole_Dialog(QString text, QLispConsole* main_console, QWidget* parent = nullptr);

 int get_current_line_number();

 void log_command(QStringList* list, QString* cmd);

 QString get_current_text();

 ~QLispConsole_Dialog();

 //void close_button_clicked();

Q_SIGNALS:
 void canceled(QDialog*);
 void accepted(QDialog*);


public Q_SLOTS:
 void accept();
 void cancel();

 void proceed();


};


#endif //  QLISPCONSOLE_DIALOG__H


