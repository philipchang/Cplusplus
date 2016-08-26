#ifndef __MAPTRIXC_BASETYPE_DEFINE__
#define __MAPTRIXC_BASETYPE_DEFINE__
//	MaptrixC Template Library

typedef char					CHAR;
typedef int						INT;
typedef int						SINT;
typedef long					LONG;
typedef unsigned char			UCHAR;
typedef unsigned int			UINT;
typedef unsigned long			ULONG;

typedef char					SINT8;
typedef short					SINT16;
typedef int						SINT32;

typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int			UINT32;

#if defined _MSC_VER
	typedef __int64				SINT64;
	typedef unsigned __int64	UINT64;
#elif defined __GNUC__
	typedef long long			SINT64;
	typedef unsigned long long	UINT64;
#endif

typedef int						BOOL;
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX
#define MAX(x, y)			((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y)			((x) < (y) ? (x) : (y))
#endif

#ifndef ABS
#define ABS(x)				((x) < 0 ? -(x) : (x))
#endif

#ifndef Float2Int
#define Float2Int(x)		((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
#endif

#ifndef SIGN
#define SIGN(x)				(((x) < 0) ? -1 : (x) > 0 ? 1 : 0)
#endif

#ifndef BSIGN
#define BSIGN(x)			(((x) < 0) ? -1 : 1)
#endif

#ifndef SAME_SIGNS
#define SAME_SIGNS(x, y)	(((x) ^ (y)) >= 0)	//(((x) * (y)) >= 0)
#endif

#ifndef SQUARE
#define SQUARE(x)			((x) * (x))
#endif

#ifndef SWAP
#define SWAP
template <class _T>
inline void Swap(_T &A, _T &B)
{
	_T C = A; A = B; B = C;
}
#endif

#endif
