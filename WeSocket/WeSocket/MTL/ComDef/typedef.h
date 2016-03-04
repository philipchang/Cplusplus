
/*! 
    \file       typedef.h
    \brief      整个TBT使用的基本数据类型定义
    \details    无
	\author     yxy
*/

#ifndef __TBT_BASELIB_TYPEDEF_H__
#define __TBT_BASELIB_TYPEDEF_H__

typedef unsigned char       byte;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned int        UINT;
typedef long                LONG;
typedef int                 BOOL;
typedef unsigned long       ULONG;

typedef signed char         INT8;
typedef short               INT16;
typedef int                 INT32;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;

typedef unsigned char*      LPBYTE;

#if defined _MSC_VER || defined __BORLANDC__
	typedef __int64				S_INT64;
	typedef unsigned __int64	U_INT64;
	typedef __int64              INT64;
	typedef unsigned __int64     UINT64;
#elif defined __GNUC__
	typedef long long			S_INT64;
	typedef unsigned long long	U_INT64;
	typedef long long            INT64;
	typedef unsigned long long   UINT64;
#endif

#ifndef NULL
#define NULL				(0)
#endif

#ifndef FALSE
#define FALSE               (0)
#endif

#ifndef TRUE
#define TRUE                (1)
#endif

#endif