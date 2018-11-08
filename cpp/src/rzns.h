
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZNS__H
#define RZNS__H

#define USE_RZNS

#ifdef USE_RZNS

#define RZNS_(X) \
 namespace RZ { namespace X {

#define _RZNS(X) \
 } }


#define RZNS_CLASS_DECLARE(X ,C) \
 namespace RZ { namespace X { class C; } }


#define INNER_NS_(X) \
 namespace X{

#define END_INNER_NS(X) \
 }

#define USING_RZNS(x) \
 using namespace RZ::x;


#else

#define RZNS_(X) \

#define _RZNS(X) \

#define INNER_NS_(X) \

#define END_INNER_NS(X) \

#define USING_RZNS(x) \

#endif


#endif //RZNS__H
