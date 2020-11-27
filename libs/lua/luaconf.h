/*
** $Id: luaconf.h,v 1.259.1.1 2017/04/19 17:29:57 roberto Exp $
** Configuration file for Lua
** See Copyright Notice in lua.h
*/

#ifndef luaconf_h
#define luaconf_h

#include <limits.h>
#include <stddef.h>
#include "fix16.h"
#include "strtofix16.h"

#define LUA_FLOAT_TYPE LUA_FLOAT_FIX16
#define LUAI_BITSINT	32
#define LUA_INT_INT		1
#define LUA_INT_LONG		2
#define LUA_INT_LONGLONG	3
#define LUA_FLOAT_FLOAT		1
#define LUA_FLOAT_DOUBLE	2
#define LUA_FLOAT_LONGDOUBLE	3
#define LUA_FLOAT_FIX16   4
#define LUA_INT_TYPE	LUA_INT_LONGLONG

#define LUA_PATH_SEP            ";"
#define LUA_PATH_MARK           "?"
#define LUA_EXEC_DIR            "!"

#define LUA_VDIR	LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
#if defined(_WIN32)	/* { */
#define LUA_LDIR	"!\\lua\\"
#define LUA_CDIR	"!\\"
#define LUA_SHRDIR	"!\\..\\share\\lua\\" LUA_VDIR "\\"
#define LUA_PATH_DEFAULT  \
		LUA_LDIR"?.lua;"  LUA_LDIR"?\\init.lua;" \
		LUA_CDIR"?.lua;"  LUA_CDIR"?\\init.lua;" \
		LUA_SHRDIR"?.lua;" LUA_SHRDIR"?\\init.lua;" \
		".\\?.lua;" ".\\?\\init.lua"
#define LUA_CPATH_DEFAULT \
		LUA_CDIR"?.dll;" \
		LUA_CDIR"..\\lib\\lua\\" LUA_VDIR "\\?.dll;" \
		LUA_CDIR"loadall.dll;" ".\\?.dll"

#else			/* }{ */

#define LUA_ROOT	"/usr/local/"
#define LUA_LDIR	LUA_ROOT "share/lua/" LUA_VDIR "/"
#define LUA_CDIR	LUA_ROOT "lib/lua/" LUA_VDIR "/"
#define LUA_PATH_DEFAULT  \
		LUA_LDIR"?.lua;"  LUA_LDIR"?/init.lua;" \
		LUA_CDIR"?.lua;"  LUA_CDIR"?/init.lua;" \
		"./?.lua;" "./?/init.lua"
#define LUA_CPATH_DEFAULT \
		LUA_CDIR"?.so;" LUA_CDIR"loadall.so;" "./?.so"
#endif			/* } */


/*
@@ LUA_DIRSEP is the directory separator (for submodules).
** CHANGE it if your machine does not use "/" as the directory separator
** and is not Windows. (On Windows Lua automatically uses "\".)
*/
#if defined(_WIN32)
#define LUA_DIRSEP	"\\"
#else
#define LUA_DIRSEP	"/"
#endif

#define LUA_API		extern
#define LUALIB_API	LUA_API
#define LUAMOD_API	LUALIB_API

#if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 302) && \
    defined(__ELF__)		/* { */
#define LUAI_FUNC	__attribute__((visibility("hidden"))) extern
#else				/* }{ */
#define LUAI_FUNC	extern
#endif				/* } */

#define LUAI_DDEC	LUAI_FUNC
#define LUAI_DDEF	/* empty */

#define LUA_COMPAT_FLOATSTRING

#define lua_numbertointeger(n,p) \
	(*(p) = (LUA_INTEGER)(fix16_to_int(n)), 1)

#define lua_number2str(s,sz,n) (fix16_to_str(n, (s)))

#define LUA_NUMBER	fix16_t
#define LUAI_UACNUMBER	fix16_t
#define LUA_NUMBER_FRMLEN	""
#define LUA_NUMBER_FMT		"%i.%i"
#define LUA_INTEGER_FMT		"%" LUA_INTEGER_FRMLEN
#define LUAI_UACINT		LUA_INTEGER
#define LUA_UNSIGNED		unsigned LUAI_UACINT
#define LUA_INTEGER		short
#define LUA_INTEGER_FRMLEN	"i"

#define LUA_MAXINTEGER		SHRT_MAX
#define LUA_MININTEGER		SHRT_MIN

#define l_mathlim(n)		(FIX16_##n)
#define l_mathop(op)		fix16_##op
#define l_litint(x)			(F16(x))
#define fix16_fabs 			fix16_abs
#define fix16_fmod 			fix16_mod
#define fix16_log10(x)  (fix16_div(fix16_log(x), fix16_log(10)))
#define fix16_pow(x, y)	(fix16_from_float(powf(fix16_to_float(x), fix16_to_float(y))))
#define fix16_frexp(x, i) ((*i) = x >> 16)
#define lua_str2number(s,p)	(strtofix16(s, (p)))
#define l_floor(x)		(fix16_floor(x))

#define lua_integer2str(s,sz,n)  l_sprintf((s), sz, LUA_INTEGER_FMT, (LUAI_UACINT)(n))
#define l_sprintf(s,sz,f,i)	snprintf(s,sz,f,i)

#define lua_strx2number(s,p)		lua_str2number(s,p)
#define lua_pointer2str(buff,sz,p)	l_sprintf(buff,sz,"%p",p)
#define lua_number2strx(L,b,sz,f,n) ((void)L, l_sprintf(b,sz,f,(LUAI_UACNUMBER)(n)))

#define LUA_KCONTEXT	ptrdiff_t
#define lua_getlocaledecpoint()		(localeconv()->decimal_point[0])

#define LUAI_MAXSTACK		1000000
#define LUA_EXTRASPACE		(sizeof(void *))
#define LUA_IDSIZE	60
#define LUAL_BUFFERSIZE   ((int)(0x80 * sizeof(void*) * sizeof(lua_Integer)))
#define FIX16_MAX_10_EXP 5
#define FIX16_MANT_DIG 16

#endif