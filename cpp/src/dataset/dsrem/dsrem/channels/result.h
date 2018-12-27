
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RESULT__H
#define RESULT__H

#include <QPair>

template<
  typename T1,
  typename T2 = _info,
  typename T3 = _temp
  >
struct Result1 : std::tuple<T1&, T2, T3>
{
 Result1(T1& arg)
  : std::tuple<T1&, T2, T3>(arg, _info(), _temp())
 {

 }
};




#endif // RESULT__H
