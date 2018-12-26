
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>

#include "dsrem/channels/lambda.h"
#include "dsrem/channels/result.h"

void test(Lambda<QString, int> lc, Result<int>& rc)
{
 QString& text = lc.first;
 int& count = lc.second;
 int& result = rc;

 qDebug() << "Text: " << text;
 qDebug() << "Count: " << count;

 result = 77;
}

int main(int argc, char* argv[])
{
 int r;
 test({"Ok", 44}, r);

 qDebug() << "r: " << r;

 return 0;
}
