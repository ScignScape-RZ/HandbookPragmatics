
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DEMO__H
#define DEMO__H


#include <QVector>
#include <QString>



class Demo
{
 QVector<QStringList*>* groups_;

public:

 Demo();

 QVector<QStringList*>* groups() { return groups_; }

};


#endif // DEMO__H
