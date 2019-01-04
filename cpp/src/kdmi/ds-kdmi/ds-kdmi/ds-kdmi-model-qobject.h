
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DS_KDMI_MODEL_QOBJECT__H
#define DS_KDMI_MODEL_QOBJECT__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>
#include <QObject>


KANS_CLASS_DECLARE(KDMI, KDMI_Model)

USING_KANS(KDMI)

class KDMI_Model_QObject : public QObject
{
Q_OBJECT

 KDMI_Model* kdmi_model_;

public:

 KDMI_Model_QObject(KDMI_Model* kdmi_model);

 Q_INVOKABLE void init_model_from_files(QString s, QString g);


};


#endif // DS_KDMI_MODEL_QOBJECT__H
