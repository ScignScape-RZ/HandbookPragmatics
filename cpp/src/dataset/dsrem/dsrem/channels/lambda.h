
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef LAMBDA__H
#define LAMBDA__H

#include <QPair>

struct _no_arg
{
};

struct _info
{
};

struct _temp
{
};

template<typename... Ts>
using tup = std::tuple<Ts...>;

template<
  typename T1 = _info,
  typename T2 = _temp
  >
struct Lambda0 : std::tuple<T1, T2>
{
 Lambda0()
 {}
};

template<
  typename T1,
  typename T2 = _info,
  typename T3 = _temp
  >
struct Lambda1 : std::tuple<T1, T2, T3>
{
 Lambda1(T1 arg)
  : std::tuple<T1, T2, T3>(arg, _info(), _temp())
 {

 }
};

template<
  typename T1,
  typename T2,
  typename T3 = _info,
  typename T4 = _temp
  >
struct Lambda2 : std::tuple<T1, T2, T3, T4>
{
 Lambda2(std::tuple<T1, T2> args)
  : std::tuple<T1, T2, T3, T4>(std::get<0>(args),
    std::get<1>(args), _info(), _temp())
 {

 }
};



//template<
//  typename T1 = _no_arg,
//  typename T2 = _no_arg,
//  typename T3 = _no_arg,
//  typename T4 = _no_arg,
//  typename T5 = _no_arg,
//  typename T6 = _no_arg,
//  typename T7 = _no_arg,
//  typename T8 = _no_arg,
//  typename T9 = _info,
//  typename T10 = _temp
//  >
//struct Lambda8 : std::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
//{

//};


//template<
//  typename T1 = _no_arg,
//  typename T2 = _no_arg,
//  typename T3 = _no_arg,
//  typename T4 = _no_arg,
//  typename T5 = _no_arg,
//  typename T6 = _no_arg,
//  typename T7 = _no_arg,
//  typename T8 = _info,
//  typename T9 = _temp
//  >
//struct Lambda7 : std::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
//{

//}



#endif // LAMBDA__H
