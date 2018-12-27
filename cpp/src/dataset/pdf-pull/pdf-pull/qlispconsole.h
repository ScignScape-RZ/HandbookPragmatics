

//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


/*
  QPyConsole.h

  QConsole specialization for Python

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

#ifndef QLISPCONSOLE_H
#define QLISPCONSOLE_H

#include "qconsole.h"

#include "accessors.h"

#include <functional>

class QLispConsole : public QConsole
{
public:
    //destructor
    ~QLispConsole();

    //get the QLispConsole instance
    static QLispConsole* getInstance(QWidget *parent = NULL,
                                   const QString& welcomeText = "");

    void printHistory();

protected:
    //give suggestions to complete a command (not working...)
    QStringList suggestCommand(const QString &cmd, QString& prefix);

    //private constructor
    QLispConsole(QWidget *parent = NULL,
               const QString& welcomeText = "");

    //execute a validated command
    QString interpretCommand(const QString &command, int *res);

    void setNormalPrompt(bool display) { setPrompt(">>", display); }
    void setMultilinePrompt(bool display) { setPrompt("...", display); }

private:

    //The instance
    static QLispConsole* theInstance;

    std::function<void(const QString&, int*, QString&)> cb_;

public:

  ACCESSORS(std::function<void(const QString&, int*, QString&)> ,cb)

private:
    // function to check if current command compiled and if not hinted for a multiline statement
    bool lisp_check_for_unexpected_eof();

    // string holding the current command
    QString command;

    // number of lines associated with current command
    int lines;
};

#endif
