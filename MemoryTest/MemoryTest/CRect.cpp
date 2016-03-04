#include "CRect.h"

CWPoint2Di::CWPoint2Di( void ):x(0),y(0)
{	
}

CWPoint2Di::CWPoint2Di( uint32 xx, uint32 yy ):x(xx),y(yy)
{	
}

CWPoint2Di::~CWPoint2Di()
{

}

bool CWPoint2Di::operator == ( const CWPoint2Di& pt ) const
{
	return( x == pt.x && y == pt.y );
}

bool CWPoint2Di::operator != ( const CWPoint2Di& pt ) const
{
	return( x != pt.x || y != pt.y );
}

float CWPoint2Di::Dist( const CWPoint2Di& pt ) const
{
	return 0;
}

float CWPoint2Di::Azimuth( const CWPoint2Di& pt ) const
{
	double dAngle = 0;
	return( dAngle );
}

float CWPoint2Di::Azimuth( signed int x1, signed int y1, signed int x2, signed int y2)
{
	double dAngle = 0;
	return( dAngle );
}

CWRect2Di::CWRect2Di():left(0),top(0),right(0),bottom(0)
{
}

CWRect2Di::CWRect2Di( int32 x,int32 y ):left(x),right(x),top(y),bottom(y)
{	
}

CWRect2Di::CWRect2Di( int32 l,int32 t,int32 r,int32 b ):left(l),right(r),top(t),bottom(b)
{
}

CWRect2Di::CWRect2Di( const CWPoint2Di& pt1,const CWPoint2Di& pt2 )
{
	if ( pt1.x <= pt2.x )
	{
		left = pt1.x;			right = pt2.x;
	}else
	{
		left = pt2.x;			right = pt1.x;
	}

	if ( pt1.y <= pt2.y )
	{
		bottom = pt1.y;			top = pt2.y;
	}else
	{
		bottom = pt2.y;			top = pt1.y;
	}	
}

CWRect2Di::CWRect2Di( const CWPoint2Di& pt,uint16 w, uint16 h )
{	
}

bool CWRect2Di::operator == ( const CWRect2Di& rc )
{
	if ( left != rc.left )		return(false);
	if ( right != rc.right )	return(false);
	if ( top != rc.top )		return(false);
	if ( bottom != rc.bottom )	return(false);

	return(true);
}
CWRect2Di& CWRect2Di::operator = ( const CWRect2Di& rc )
{
	if(this != &rc)
	{
		memcpy(this, &rc, sizeof(CWRect2Di));
	}
	return *this;
}

int32 CWRect2Di::Width()	{ return abs(left-right); };
int32 CWRect2Di::Height() { return abs(top-bottom); };

void CWRect2Di::Inflate( uint16 sx, uint16 sy )
{
	left -= sx;	right += sx;
	top += sy;	bottom -= sy;
}

bool CWRect2Di::IsRectEmpty()
{
	if ( (abs(left - top) < EP) && (abs(right - bottom) < EP) && (abs(top - bottom) < EP) )
	{
		return(true);
	}
	return(false);
}

bool CWRect2Di::IsRectNull()
{
	if( (abs(left - 0) < EP) && 
		(abs(right - 0) < EP) && 
		(abs(top - 0) < EP) && 
		(abs(bottom - 0) < EP) )
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool CWRect2Di::IsIntersect( const CWPoint2Di& pt1,const CWPoint2Di& pt2 ) const
{
	/* 线段有一个端点或两个端点在矩形内 */
	if ( PtInRect( pt1 ) || PtInRect( pt2 ) )
	{
		return(true);
	}

	/* 线段在矩形外 */
	if ( pt1.x < left && pt2.x < left )
	{
		return(false);
	}
	if ( pt1.x > right && pt2.x > right )
	{
		return(false);
	}
	if ( pt1.y > top && pt2.y > top )
	{
		return(false);
	}
	if ( pt1.y < bottom && pt2.y < bottom )
	{
		return(false);
	}

	/* 线段与矩形水平或垂直交叉 */
	if ( left <= pt1.x && pt1.x <= right && 
		left <= pt2.x && pt2.x <= right )
	{
		if ( pt1.y < top && pt2.y > bottom )
		{
			return(true);
		}
		if ( pt2.y < top && pt1.y > bottom )
		{
			return(true);
		}
	}
	if ( top >= pt1.y && pt1.y >= bottom && 
		top >= pt2.y && pt2.y >= bottom )
	{
		if ( pt1.x < left && pt2.x > right )
		{
			return(true);
		}
		if ( pt2.x < left && pt1.x > right )
		{
			return(true);
		}
	}

	/* 判断线段与矩形实际的交叉情况 */
	register float v1x = pt2.x - pt1.x;
	register float v1y = pt2.y - pt1.y;

	register float v2x = left - pt1.x;
	register float v2y = top  - pt1.y;
	register float nCross = v1x*v2y - v1y*v2x,nCrossTemp;
	if ( fabs(nCross - 0) < EP )
	{
		return(true);
	}

	nCross = nCross/fabs( nCross );

	v2x = right - pt1.x;
	v2y = top - pt1.y;
	nCrossTemp = v1x*v2y - v1y*v2x;	
	if ( nCross*nCrossTemp <= 0 )	/* 符号相反，说明与线段交叉 */
	{
		return(true);
	}

	v2x = right - pt1.x;
	v2y = bottom - pt1.y;
	nCrossTemp = v1x*v2y - v1y*v2x;	
	if ( nCross*nCrossTemp <= 0 )	/* 符号相反，说明与线段交叉 */
	{
		return(true);
	}

	v2x = left - pt1.x;
	v2y = bottom - pt1.y;
	nCrossTemp = v1x*v2y - v1y*v2x;	
	if ( nCross*nCrossTemp <= 0 )	/* 符号相反，说明与线段交叉 */
	{
		return(true);
	}

	return(false);
}

bool CWRect2Di::IsIntersect( const CWPoint2Di* pt,int32 nNum ) const
{
	for ( register int32 i = 0L; i < nNum-1; i++ )
	{
		if ( IsIntersect( pt[i], pt[i+1] ) == true )
		{
			return(true);
		}
	}

	return(false);
}

bool CWRect2Di::IsIntersect( const CWRect2Di& rc ) const
{
	if(right < rc.left)//if(right <= rc.left)
	{
		return  false;	
	}
	else  if(left > rc.right)//else  if(left >= rc.right)
	{
		return  false;	
	}
	else  if(top < rc.bottom)//else  if(top <= rc.bottom)
	{
		return  false;		
	}
	else  if(bottom > rc.top)//else  if(bottom >= rc.top)
	{
		return  false;	
	}
	return  true;    
}

bool CWRect2Di::PtInRect( const CWPoint2Di& point ) const
{
	if( point.x <= left )		return false;
	else if( point.x >= right )	return false;
	else if( point.y >= top )	return false;
	else if( point.y <= bottom )return false;
	else	return true;
}

void CWRect2Di::Intersect( const CWRect2Di& rc )
{
	if( left < rc.left )			left = rc.left; 
	if( left > rc.right )			left = rc.right; 
	if( right < rc.left )			right = rc.left; 
	if( right > rc.right )			right = rc.right; 
	if( top > rc.top )				top = rc.top; 
	if( top < rc.bottom )			top = rc.bottom; 
	if( bottom > rc.top )			bottom = rc.top; 
	if( bottom < rc.bottom )		bottom = rc.bottom; 
}

void CWRect2Di::Union( const CWRect2Di& rc )
{
	if ( left > rc.left )			left = rc.left;
	if ( right < rc.right )			right = rc.right;
	if ( top < rc.top )				top = rc.top;
	if ( bottom > rc.bottom )		bottom = rc.bottom;
}

void CWRect2Di::Union( const CWPoint2Di& pt )
{
	if ( left > pt.x )			left = pt.x;
	if ( right < pt.x )			right = pt.x;
	if ( top < pt.y )			top = pt.y;
	if ( bottom > pt.y )		bottom = pt.y;
}

void CWRect2Di::Union( int32 x, int32 y )
{
	if ( left > x )			left = x;
	if ( right < x )		right = x;
	if ( top < y )			top = y;
	if ( bottom > y )		bottom = y;
}

void CWRect2Di::NomalizeRect( void )
{
	int32 i4Temp;

	if( left > right )
	{
		i4Temp = left;
		left = right;
		right = i4Temp;
	}

	if( bottom > top )
	{
		i4Temp = bottom;
		bottom = top;
		top = i4Temp;
	}
}

bool CWRect2Di::IntersectRect( const CWRect2Di& rc1, const CWRect2Di& rc2 )
{
#if 0
	rc1.NomalizeRect();
	rc2.NomalizeRect();
#endif
	if( rc1.IsIntersect( rc2 ) == true )
	{
		if( rc1.left < rc2.left )
		{
			if(rc1.top > rc2.top)
			{
				left = rc2.left;
				right = rc1.right;
				top = rc2.top;
				bottom = rc1.bottom;
			}
			else
			{
				left = rc2.left;
				right = rc1.right;
				top = rc1.top;
				bottom = rc2.bottom;
			}
		}
		else
		{
			if(rc1.top > rc2.top)
			{
				left = rc1.left;
				right = rc2.right;
				top = rc2.top;
				bottom = rc1.bottom;
			}
			else
			{
				left = rc1.left;
				right = rc2.right;
				top = rc1.top;
				bottom = rc2.bottom;
			}
		}
		return true;
	}

	return false;
}
