
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

 QVBoxLayout* main_layout_;

 qint8 left_id_;
 qint8 right_id_;

 QStringList sentence_;

 void clear_buttons();

 QMap<QPair<quint8, quint8>, QPair<QString, QString>> pairs_;

 void set_button_width(QPushButton* btn);
 void check_pair();


public:

 Lexpair_Dialog(QStringList sent, QWidget* parent);
 ~Lexpair_Dialog();



Q_SIGNALS:

 void canceled(QDialog*);
 void accepted(QDialog*);


public Q_SLOTS:

 void accept();
 void cancel();
// void proceed();

};



#endif  // LEXPAIR_DIALOG__H


