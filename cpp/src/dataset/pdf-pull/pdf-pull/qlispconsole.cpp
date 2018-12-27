
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

//  derived from ...

/*
  QPyConsole.cpp

  Controls the GEMBIRD Silver Shield PM USB outlet device

  (C) 2006, Mondrian Nuessle, Computer Architecture Group, University of Mannheim, Germany

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


  nuessle@uni-mannheim.de

*/

// modified by YoungTaek Oh.

#ifdef WIN32
#   undef _DEBUG
#endif

#include "qlispconsole.h"

#include <QDebug>


QString resultString;


void QLispConsole::printHistory()
{
    uint index = 1;
    for ( QStringList::Iterator it = history.begin(); it != history.end(); ++it )
    {
        resultString.append(QString("%1\t%2\n").arg(index).arg(*it));
        index ++;
    }
}

QLispConsole* QLispConsole::theInstance = NULL;

QLispConsole* QLispConsole::getInstance(QWidget *parent, const QString& welcomeText)
{
    if (!theInstance)
    {
        theInstance = new QLispConsole(parent, welcomeText);
    }
    return theInstance;
}

//QTcl console constructor (init the QTextEdit & the attributes)
QLispConsole::QLispConsole(QWidget *parent, const QString& welcomeText) :
        QConsole(parent, welcomeText), cb_(nullptr), lines(0)
{
    //set the Python Prompt
    setNormalPrompt(true);

    /* NOTE: In previous implementaion, local name and global name
             were allocated separately.  And it causes a problem that
             a function declared in this console cannot be called.  By
             unifying global and local name with __main__.__dict__, we
             can get more natural python console.
    */
}


bool
QLispConsole::lisp_check_for_unexpected_eof()
{
    return false;
}

//Desctructor
QLispConsole::~QLispConsole()
{
}

QString QLispConsole::interpretCommand(const QString &command, int* res)
{
 QConsole::interpretCommand(command, res);
 if(cb_)
 {
  QString result;
  cb_(command, res, result);
  return result;
 }
 return "";
}

QStringList QLispConsole::suggestCommand(const QString &cmd, QString& prefix)
{
 return {};
}
