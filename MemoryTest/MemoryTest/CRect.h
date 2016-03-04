
#pragma once

#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define uint16 unsigned short
#define uint32 unsigned int
#define int32 signed int

	/* 四舍五入宏 */ 
	#ifndef ROUND
	#define ROUND( x ) ( ((x)-((long)(x)))>(0.5) ? (long(x)+1):long(x) )
	#endif

	/* 常用的常量定义 */ 
	const long MAX_LONG = 2147483647L;
	const double INF = 1E200;    /* 无限值 */
	const double EP = 1E-10;	 /* 最小值 */
	const short MAXV = 300; 
	const double PI = 3.1415926535; 


/* 整数形式表示的WGS84坐标，1/16秒(单位)*/
class  CWPoint2Di
{
public:
	CWPoint2Di( void );
	explicit CWPoint2Di( unsigned int xx, unsigned int yy );
	~CWPoint2Di();

	float Dist( const CWPoint2Di& pt ) const;
	float Azimuth( const CWPoint2Di& pt ) const;
	static float Azimuth( signed int x1, signed int y1, signed int x2, signed int y2);

	bool operator == ( const CWPoint2Di& pt ) const;
	bool operator != ( const CWPoint2Di& pt ) const;

	signed int x,y;
};

class  CWRect2Di
{
public:
	CWRect2Di();
	CWRect2Di( signed int x,signed int y );
	CWRect2Di( signed int l,signed int t,signed int r,signed int b );
	CWRect2Di( const CWPoint2Di& pt1,const CWPoint2Di& pt2 );
	CWRect2Di( const CWPoint2Di& pt,uint16 w, uint16 h );

	bool operator == ( const CWRect2Di& rc );
	CWRect2Di& operator = ( const CWRect2Di& rc );

	void NomalizeRect( void );

	signed int Width();
	signed int Height();

	void Inflate( unsigned short sz, unsigned short sy );

	bool IsRectEmpty();
	bool IsRectNull();

	bool IsIntersect( const CWPoint2Di& pt1,const CWPoint2Di& pt2 ) const;	
	bool IsIntersect( const CWPoint2Di* pt,signed int nNum ) const;
	bool IsIntersect( const CWRect2Di& rc ) const;

	bool PtInRect( const CWPoint2Di& point ) const;
	void Intersect( const CWRect2Di& rc );
	void Union( const CWRect2Di& rc );
	void Union( const CWPoint2Di& pt );
	void Union( int32 x, int32 y );

	bool IntersectRect( const CWRect2Di& rc1, const CWRect2Di& rc2 );

	signed int left,top,right,bottom;
};
