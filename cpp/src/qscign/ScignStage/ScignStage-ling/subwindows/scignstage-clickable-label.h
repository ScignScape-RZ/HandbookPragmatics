
//          Copyright Nathaniel Christen 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef SCIGNSTAGE_CLICKABLE_LABEL__H
#define SCIGNSTAGE_CLICKABLE_LABEL__H

#include <QLabel>

#include "qsns.h"

#include "accessors.h"

#include <functional>

//QSNS_(ScignStage)
//?namespace QScign { namespace ScignStage {


class ScignStage_Clickable_Label : public QLabel
{
 Q_OBJECT

 QString text_data_;

 std::function<void(QObject*, QString)> cb_;
 typedef std::function<void(QObject*, QString)> cb_type;

public:

 explicit ScignStage_Clickable_Label(QWidget* parent = nullptr,
   Qt::WindowFlags f = Qt::WindowFlags());

 ~ScignStage_Clickable_Label();


 ACCESSORS(QString ,text_data)
 ACCESSORS(cb_type ,cb)


Q_SIGNALS:
 void clicked();

protected:
  void mousePressEvent(QMouseEvent* event);

};

//_QSNS(ScignStage)

#endif  // SCIGNSTAGE_CLICKABLE_LABEL__H
