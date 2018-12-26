
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QString>
#include <QDebug>

#include "dsrem/channels/lambda.h"
#include "dsrem/channels/result.h"

void test(Lambda2<QString, int> lc, Result<int>& rc)
{
 QString& text = std::get<0>(lc);
 int& count = std::get<1>(lc);
 int& result = std::get<0>(rc);

 qDebug() << "Text: " << text;
 qDebug() << "Count: " << count;

 result = 77;
}



int main(int argc, char* argv[])
{
 int r = 9;
 Result<int> rc = std::make_tuple(r);
 //Lambda2<QString, int> lc(std::tuple<QString, int>{"Ok", 44});
 test(tup<QString, int>{"Ok", 44}, rc);

 //test(std::tuple<QString, int>{"Ok", 44}, rc);


 qDebug() << "r: " << r;

 return 0;
}
