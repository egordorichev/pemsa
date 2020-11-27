/*
** $Id: llimits.h,v 1.141.1.1 2017/04/19 17:20:42 roberto Exp $
** Limits, basic types, and some other 'installation-dependent' definitions
** See Copyright Notice in lua.h
*/

#ifndef llimits_h
#define llimits_h

#include <limits.h>
#include <stddef.h>
#include "lua.h"

typedef size_t lu_mem;
typedef ptrdiff_t l_mem;
typedef unsigned char lu_byte;


#define MAX_SIZET	((size_t)(~(size_t)0))
#define MAX_SIZE	(sizeof(size_t) < sizeof(lua_Integer) ? MAX_SIZET : (size_t)(LUA_MAXINTEGER))
#define MAX_LUMEM	((lu_mem)(~(lu_mem)0))
#define MAX_LMEM	((l_mem)(MAX_LUMEM >> 1))
#define MAX_INT		INT_MAX

#define point2uint(p)	((unsigned int)((size_t)(p) & UINT_MAX))

typedef union {
  lua_Number n;
  double u;
  void *s;
  lua_Integer i;
  long l;
} L_Umaxalign;

typedef LUAI_UACNUMBER l_uacNumber;
typedef LUAI_UACINT l_uacInt;

#define lua_assert(c)		((void)0)
#define check_exp(c,e)		(e)
#define lua_longassert(c)	((void)0)

#define luai_apicheck(l,e)	lua_assert(e)
#define api_check(l,e,msg)	luai_apicheck(l,(e) && msg)

#if !defined(UNUSED)
#define UNUSED(x)	((void)(x))
#endif


#define cast(t, exp)	((t)(exp))

#define cast_void(i)	cast(void, (i))
#define cast_byte(i)	cast(lu_byte, (i))
#define cast_int(i)	cast(int, (i))
#define cast_uchar(i)	cast(unsigned char, (i))
#define cast_num(i) fix16_from_int(cast(int, (i)))
#define l_castS2U(i)	((lua_Unsigned)(i))
#define l_castU2S(i)	((lua_Integer)(i))

#if defined(__GNUC__)
#define l_noret		void __attribute__((noreturn))
#elif defined(_MSC_VER) && _MSC_VER >= 1200
#define l_noret		void __declspec(noreturn)
#else
#define l_noret		void
#endif

#if !defined(LUAI_MAXCCALLS)
#define LUAI_MAXCCALLS		200
#endif

typedef unsigned int Instruction;

#define LUAI_MAXSHORTLEN	40
#define MINSTRTABSIZE	128

#if !defined(STRCACHE_N)
#define STRCACHE_N		53
#define STRCACHE_M		2
#endif

#if !defined(LUA_MINBUFFER)
#define LUA_MINBUFFER	32
#endif

#if !defined(lua_lock)
#define lua_lock(L)	((void) 0)
#define lua_unlock(L)	((void) 0)
#endif

#if !defined(luai_threadyield)
#define luai_threadyield(L)	{lua_unlock(L); lua_lock(L);}
#endif


#if !defined(luai_userstateopen)
#define luai_userstateopen(L)		((void)L)
#endif

#if !defined(luai_userstateclose)
#define luai_userstateclose(L)		((void)L)
#endif

#if !defined(luai_userstatethread)
#define luai_userstatethread(L,L1)	((void)L)
#endif

#if !defined(luai_userstatefree)
#define luai_userstatefree(L,L1)	((void)L)
#endif

#if !defined(luai_userstateresume)
#define luai_userstateresume(L,n)	((void)L)
#endif

#if !defined(luai_userstateyield)
#define luai_userstateyield(L,n)	((void)L)
#endif

#define luai_numdiv(L,a,b)      (fix16_div(a, b))
#define luai_numfdiv(L,a,b)      (fix16_fdiv(a, b))

#define luai_nummod(L,a,b,m) { (m) = l_mathop(fmod)(a,b); if ((luai_nummul(L,m,b)) < 0) (m) += (b); }
#define luai_numpow(L,a,b)      ((void)L, l_mathop(pow)(a,b))

#define luai_numadd(L,a,b)      (fix16_add(a, b))
#define luai_numsub(L,a,b)      (fix16_sub(a, b))
#define luai_nummul(L,a,b)      (fix16_mul(a, b))
#define luai_numunm(L,a)        (-(a))
#define luai_numeq(a,b)         ((a)==(b))
#define luai_numlt(a,b)         ((a)<(b))
#define luai_numle(a,b)         ((a)<=(b))
#define luai_numisnan(a)        (!luai_numeq((a), (a)))

#if !defined(HARDSTACKTESTS)
#define condmovestack(L,pre,pos)	((void)0)
#else
/* realloc stack keeping its size */
#define condmovestack(L,pre,pos)  \
	{ int sz_ = (L)->stacksize; pre; luaD_reallocstack((L), sz_); pos; }
#endif

#if !defined(HARDMEMTESTS)
#define condchangemem(L,pre,pos)	((void)0)
#else
#define condchangemem(L,pre,pos)  \
	{ if (G(L)->gcrunning) { pre; luaC_fullgc(L, 0); pos; } }
#endif
#endif