
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RESULT__H
#define RESULT__H

#include <QPair>

template<typename... Ts>
using Result = std::tuple<Ts...>;



#endif // RESULT__H
