#ifndef __MAPTRIXC_BASETYPE_DEF__
#define __MAPTRIXC_BASETYPE_DEF__

typedef int						SINT;
typedef unsigned int			UINT;

typedef int						BOOL;
typedef unsigned char			BYTE;
typedef unsigned short			WORD;

typedef const char*				CSTR;
typedef       char*				PSTR;
typedef const void*				CPTR;

typedef double					REAL;
typedef short					HALF;

typedef char					SINT08;
typedef short					SINT16;
typedef int						SINT32;

typedef unsigned char			UINT08;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;

#if defined _MSC_VER
	#if _MSC_VER <= 1200		// vc 6.0
	typedef __int64				SINT64;
	typedef unsigned __int64	UINT64;
	#else
	typedef long long			SINT64;
	typedef unsigned long long	UINT64;
	#endif
#elif defined __GNUC__
	typedef long long			SINT64;
	typedef unsigned long long	UINT64;
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif
