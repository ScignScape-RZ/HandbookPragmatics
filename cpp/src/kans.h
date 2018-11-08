
//           Copyright Nathaniel Christen 2018.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef KANS__H
#define KANS__H

#define USE_KANS

#ifdef USE_KANS

#define KANS_(X) \
 namespace KA { namespace X {

#define _KANS(X) \
 } }


#define KANS_CLASS_DECLARE(X ,C) \
 namespace KA { namespace X { class C; } }


#define INNER_NS_(X) \
 namespace X{

#define END_INNER_NS(X) \
 }

#define USING_KANS(x) \
 using namespace KA::x;


#else

#define KANS_(X) \

#define _KANS(X) \

#define INNER_NS_(X) \

#define END_INNER_NS(X) \

#define USING_KANS(x) \

#endif


#endif //KANS__H
