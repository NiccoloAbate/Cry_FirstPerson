#pragma once

#define DEFINE #define
#define UNDEF #undef

/*#define __ITERATE(Num) X(##Num)
#define _ITERATE1 __ITERATE(0)
#define _ITERATE2 __ITERATE(0) __ITERATE(1)
#define _ITERATE3 __ITERATE(0) __ITERATE(1) __ITERATE(2)
#define _ITERATE4 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3)
#define _ITERATE5 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4)
#define _ITERATE6 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4) __ITERATE(5)
#define _ITERATE7 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4) __ITERATE(5) __ITERATE(6)
#define _ITERATE8 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4) __ITERATE(5) __ITERATE(6) __ITERATE(7)
#define _ITERATE9 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4) __ITERATE(5) __ITERATE(6) __ITERATE(7) __ITERATE(8)
#define _ITERATE10 __ITERATE(0) __ITERATE(1) __ITERATE(2) __ITERATE(3) __ITERATE(4) __ITERATE(5) __ITERATE(6) __ITERATE(7) __ITERATE(8) __ITERATE(9)
#define ITERATE(Num) _ITERATE##Num*/

#define _REPEAT0(x)
#define _REPEAT1(x) x
#define _REPEAT2(x) x x 
#define _REPEAT3(x) x x x 
#define _REPEAT4(x) x x x x 
#define _REPEAT5(x) x x x x x 
#define _REPEAT6(x) x x x x x x 
#define _REPEAT7(x) x x x x x x x 
#define _REPEAT8(x) x x x x x x x x 
#define _REPEAT9(x) x x x x x x x x x 
#define _REPEAT10(x) x x x x x x x x x x
#define REPEAT(Code, Num) _REPEAT##Num(Code)

#ifndef JOIN
#define JOIN(x, y) x##y
#endif // !JOIN

