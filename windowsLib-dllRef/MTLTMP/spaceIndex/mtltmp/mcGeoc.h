#ifndef __MAPTRIXC_2D_GEOGRAPHIC_FOUNDATION_CLASS__
#define __MAPTRIXC_2D_GEOGRAPHIC_FOUNDATION_CLASS__
//	MaptrixC Template Library

#include "mcBaseDef.h"

#ifndef PI
#define PI			3.1415926535897932384626433832795
#endif

#ifndef EARTHRADIUS
#define EARTHRADIUS	6370997		// 6378137 6356752
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG	57.295779513082320876798154814114
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD	0.017453292519943295769236907684883
#endif

namespace MTLTMP
{

template <class _T>
class mcGeoc
{
public:
	// 两点间的距离
	static _T GetDistance(_T *PntA, _T *PntB);
	static _T GetDistance(_T *PntA, _T *PntB, _T Rate);

	// 点到直线的距离，PntI是垂线交点
	static _T Pt2LineDist(_T *Line, _T *PntA, _T *PntI);
	static _T Pt2LineDist(_T *Line, _T *PntA, _T *PntI, _T Rate);

	// 点到矩形的距离
	static _T Pt2RectDist(_T *Rect, _T *PntA);
	static _T Pt2RectDist(_T *Rect, _T *PntA, _T Rate);

	// 获得面积
	static _T GetArea(_T *Src);
	// 获得周长
	static _T GetPerimeter(_T *Src);
	// 相交矩形的面积
	static _T IntersectArea(_T *Src, _T *Dst);
	// 合并矩形的面积
	static _T CombineArea(_T *Src, _T *Dst);

	// 矩形是否被包含
	static BOOL IsRcInside(_T *Src, _T *Dst);
	// 点是否被包含
	static BOOL IsPtInside(_T *Src, _T *Dst);
	// 点是否被包含在多边形
	static BOOL IsPtInside(_T *Src, _T *Dst, long Len);
	// 两个矩形是否相交
	static BOOL Intersects(_T *Src, _T *Dst);

	// 获取中心点
	static void GetCenter(_T *Src, _T *Dst);
	// 将  点Src和矩形Dst合并，结果存放在Dst
	static void CombinePt(_T *Src, _T *Dst);
	// 将  点Src和矩形Mbr合并，结果存放在Dst
	static void CombinePt(_T *Src, _T *Mbr, _T *Dst);
	// 将矩形Src和矩形Dst合并，结果存放在Dst
	static void CombineRc(_T *Src, _T *Dst);
	// 将矩形Src和矩形Mbr合并，结果存放在Dst
	static void CombineRc(_T *Src, _T *Mbr, _T *Dst);

	// 两条直线相交
	enum
	{
		DONT_INTERSECT = 0,
		LINE_INTERSECT = 1,
		LINE_COLLINEAR = 2
	};
	static long LineIntersect(_T *LineA, _T *LineB, _T *PntI);

	// 两条线段夹角转向，X1,Y1->X2,Y2->X3,Y2
	// > 0 左转、逆时针；< 0 右转、顺时针
	static long TurningOfLine(_T *LineT);
	static long TurningOfLine(_T X1, _T Y1, _T X2, _T Y2, _T X3, _T Y3);

	static long TurningOfArea(_T *LineT, long SizeT);
};


/////////////////////////////////////////////////////////////////////////////
//
// mcGeoc
//
/////////////////////////////////////////////////////////////////////////////

template <class _T>
_T mcGeoc<_T>::GetDistance(_T *PntA, _T *PntB)
{
	_T DimA = PntA[0] - PntB[0];
	_T DimB = PntA[1] - PntB[1];

	return DimA * DimA + DimB * DimB;
}

template <class _T>
_T mcGeoc<_T>::GetDistance(_T *PntA, _T *PntB, _T Rate)
{
	_T DimA = PntA[0] - PntB[0];
	_T DimB = PntA[1] - PntB[1];

	return DimA * DimA * Rate + DimB * DimB;
}

template <class _T>
_T mcGeoc<_T>::Pt2LineDist(_T *Line, _T *PntA, _T *PntI)
{
	_T dX = Line[2] - Line[0];
	_T dY = Line[3] - Line[1];

	_T dR = -(Line[1] - PntA[1]) * dY - (Line[0] - PntA[0]) * dX;

	_T dL;
	if (dR <= (_T)0)
	{
		PntI[0] = Line[0];
		PntI[1] = Line[1];
	}
	else if (dR >= (dL = dX * dX + dY * dY))
	{
		PntI[0] = Line[2];
		PntI[1] = Line[3];
	}
	else
	{
		PntI[0] = Line[0] + dR * dX / dL;
		PntI[1] = Line[1] + dR * dY / dL;
	}

	dX = PntA[0] - PntI[0];
	dY = PntA[1] - PntI[1];
	return dX * dX + dY * dY;
}

template <class _T>
_T mcGeoc<_T>::Pt2LineDist(_T *Line, _T *PntA, _T *PntI, _T Rate)
{
	_T dX = Line[2] - Line[0];
	_T dY = Line[3] - Line[1];

	_T dR = -(Line[1] - PntA[1]) * dY - (Line[0] - PntA[0]) * dX * Rate;

	_T dL;
	if (dR <= (_T)0)
	{
		PntI[0] = Line[0];
		PntI[1] = Line[1];
	}
	else if (dR >= (dL = dX * dX * Rate + dY * dY))
	{
		PntI[0] = Line[2];
		PntI[1] = Line[3];
	}
	else
	{
		PntI[0] = Line[0] + dR * dX / dL;
		PntI[1] = Line[1] + dR * dY / dL;
	}

	dX = PntA[0] - PntI[0];
	dY = PntA[1] - PntI[1];
	return dX * dX * Rate + dY * dY;
}

template<class _T>
_T mcGeoc<_T>::Pt2RectDist(_T *Rect, _T *PntA)
{
	_T Ret = (_T)0;

	_T Tmp;
	if (PntA[0] < Rect[0])
	{
		Tmp = Rect[0] - PntA[0];
		Ret += Tmp * Tmp;
	}
	else if (PntA[0] > Rect[2])
	{
		Tmp = PntA[0] - Rect[2];
		Ret += Tmp * Tmp;
	}

	if (PntA[1] < Rect[1])
	{
		Tmp = Rect[1] - PntA[1];
		Ret += Tmp * Tmp;
	}
	else if (PntA[1] > Rect[3])
	{
		Tmp = PntA[1] - Rect[3];
		Ret += Tmp * Tmp;
	}

	return Ret;	
}

template<class _T>
_T mcGeoc<_T>::Pt2RectDist(_T *Rect, _T *PntA, _T Rate)
{
	_T Ret = (_T)0;

	_T Tmp;
	if (PntA[0] < Rect[0])
	{
		Tmp = Rect[0] - PntA[0];
		Ret += Tmp * Tmp * Rate;
	}
	else if (PntA[0] > Rect[2])
	{
		Tmp = PntA[0] - Rect[2];
		Ret += Tmp * Tmp * Rate;
	}

	if (PntA[1] < Rect[1])
	{
		Tmp = Rect[1] - PntA[1];
		Ret += Tmp * Tmp;
	}
	else if (PntA[1] > Rect[3])
	{
		Tmp = PntA[1] - Rect[3];
		Ret += Tmp * Tmp;
	}

	return Ret;	
}

template<class _T>
_T mcGeoc<_T>::GetArea(_T *Src)
{
	return (Src[2] - Src[0]) * (Src[3] - Src[1]);
}

template<class _T>
_T mcGeoc<_T>::GetPerimeter(_T *Src)
{
	return (Src[2] - Src[0]) + (Src[3] - Src[1]);
}

template <class _T>
_T mcGeoc<_T>::IntersectArea(_T *Src, _T *Dst)
{
	if (Intersects(Src, Dst))
	{
		return (MIN(Src[2], Dst[2]) - MAX(Src[0], Dst[0])) *
			   (MIN(Src[3], Dst[3]) - MAX(Src[1], Dst[1]));
	}
//	else
		return (_T)0;
}

template <class _T>
_T mcGeoc<_T>::CombineArea(_T *Src, _T *Dst)
{
	return (MAX(Src[2], Dst[2]) - MIN(Src[0], Dst[0])) *
		   (MAX(Src[3], Dst[3]) - MIN(Src[1], Dst[1]));
}

template<class _T>
BOOL mcGeoc<_T>::IsRcInside(_T *Src, _T *Dst)
{
	if (Dst[0] > Src[0] || Dst[1] > Src[1] ||
		Dst[2] < Src[2] || Dst[3] < Src[3])
		return FALSE;
	return TRUE;
}

template <class _T>
BOOL mcGeoc<_T>::IsPtInside(_T *Src, _T *Dst)
{
	if (Dst[0] > Src[0] || Dst[1] > Src[1] ||
		Dst[2] < Src[0] || Dst[3] < Src[1])
		return FALSE;
	return TRUE;
}

template <class _T>
BOOL mcGeoc<_T>::IsPtInside(_T *Src, _T *Dst, long Len)
{
#if 0
	_T X0 = Src[0];
	_T Y0 = Src[1];
	_T X1 = Dst[0];
	_T Y1 = Dst[1];

	BOOL b = 0;
	for(int i = 1; i < Len; i++)
	{
		Dst += 2;
		_T X2 = Dst[0];
		_T Y2 = Dst[1];

		if (Y1 <= Y0 && Y0 <= Y2 ||
			Y2 <= Y0 && Y0 <= Y1)
		{
			if ((X2 - X1) * (Y0 - Y1) <= (X0 - X1) * (Y2 - Y1))
				b = !b;
		}
		X1 = X2;
		Y1 = Y2;
	}

	return b;
#endif

	_T X0 = src[0];
	_T Y0 = src[1];
	_T X1 = dst[0];
	_T Y1 = dst[1];

	BOOL b = 0;
	for(int i = 1; i < len; i++)
	{
		dst += 2;
		_T X2 = dst[0];
		_T Y2 = dst[1];

		_T dY = Y2 - Y1;
		if (dY == 0)
		{
		//	if (X1 <= X0 && X0 <= X2 ||
		//		X2 <= X0 && X0 <= X1)
		//		return TRUE;
		}
		else
		{
			if (Y1 <= Y0 && Y0 <= Y2 ||
				Y2 <= Y0 && Y0 <= Y1)
			{
				_T ll = (X2 - X1) * (Y0 - Y1);
				_T rr = (X0 - X1) * (dY);

			//	if (ll == rr)
			//		return TRUE;

				if (dY > 0 && ll > rr || dY < 0 && ll < rr)
					b = !b;
			}
		}

		X1 = X2;
		Y1 = Y2;
	}

	return b;

}

template <class _T>
BOOL mcGeoc<_T>::Intersects(_T *Src, _T *Dst)
{
	if (Src[0] > Dst[2] || Src[1] > Dst[3] ||
		Src[2] < Dst[0] || Src[3] < Dst[1])
		return FALSE;
	return TRUE;
}

template <class _T>
void mcGeoc<_T>::GetCenter(_T *Src, _T *Dst)
{
	Dst[0] = (Src[0] + Src[2]) / 2;
	Dst[1] = (Src[1] + Src[3]) / 2;
}

template <class _T>
void mcGeoc<_T>::CombinePt(_T *Src, _T *Dst)
{
	Dst[0] = MIN(Src[0], Dst[0]);
	Dst[1] = MIN(Src[1], Dst[1]);
	Dst[2] = MAX(Src[0], Dst[2]);
	Dst[3] = MAX(Src[1], Dst[3]);
}

template <class _T>
void mcGeoc<_T>::CombinePt(_T *Src, _T *Mbr, _T *Dst)
{
	Dst[0] = MIN(Src[0], Mbr[0]);
	Dst[1] = MIN(Src[1], Mbr[1]);
	Dst[2] = MAX(Src[0], Mbr[2]);
	Dst[3] = MAX(Src[1], Mbr[3]);
}

template <class _T>
void mcGeoc<_T>::CombineRc(_T *Src, _T *Dst)
{
	Dst[0] = MIN(Src[0], Dst[0]);
	Dst[1] = MIN(Src[1], Dst[1]);
	Dst[2] = MAX(Src[2], Dst[2]);
	Dst[3] = MAX(Src[3], Dst[3]);
}

template <class _T>
void mcGeoc<_T>::CombineRc(_T *Src, _T *Mbr, _T *Dst)
{
	Dst[0] = MIN(Src[0], Mbr[0]);
	Dst[1] = MIN(Src[1], Mbr[1]);
	Dst[2] = MAX(Src[2], Mbr[2]);
	Dst[3] = MAX(Src[3], Mbr[3]);
}

template<class _T>
long mcGeoc<_T>::LineIntersect(_T *LineA, _T *LineB, _T *PntI)
{
	_T A1 = LineA[3] - LineA[1];
	_T B1 = LineA[0] - LineA[2];
	_T C1 = LineA[2] * LineA[1] - LineA[0] * LineA[3];

	_T L1 = A1 * LineB[0] + B1 * LineB[1] + C1;
	_T L2 = A1 * LineB[2] + B1 * LineB[3] + C1;

	if (L1 != 0 && L2 != 0 && SAME_SIGNS(L1, L2))
		return DONT_INTERSECT;

	// A2 * X  +  B2 * Y  +  C2  =  0
	_T A2 = LineB[3] - LineB[1];
	_T B2 = LineB[0] - LineB[2];
	_T C2 = LineB[2] * LineB[1] - LineB[0] * LineB[3];

	L1 = A2 * LineA[0] + B2 * LineA[1] + C2;
	L2 = A2 * LineA[2] + B2 * LineA[3] + C2;

	if (L1 != 0 && L2 != 0 && SAME_SIGNS(L1, L2))
		return DONT_INTERSECT;

	L1 = A1 * B2 - A2 * B1;
	if (L1 == 0)
		return LINE_COLLINEAR;

	PntI[0] = (_T)(((SINT64)B1 * C2 - (SINT64)B2 * C1) / L1);
	PntI[1] = (_T)(((SINT64)A2 * C1 - (SINT64)A1 * C2) / L1);
//	PntI[0] = (B1 * C2 - B2 * C1) / L1;
//	PntI[1] = (A2 * C1 - A1 * C2) / L1;

	return LINE_INTERSECT;
}

template <class _T>
long mcGeoc<_T>::TurningOfLine(_T *LineT)
{
	_T dX1 = LineT[2] - LineT[0];
	_T dY1 = LineT[3] - LineT[1];
	_T dX2 = LineT[4] - LineT[2];
	_T dY2 = LineT[5] - LineT[3];

	return dX1 * dY2 - dX2 * dY1;
}

template <class _T>
long mcGeoc<_T>::TurningOfLine(_T X1, _T Y1, _T X2, _T Y2, _T X3, _T Y3)
{
	_T dX1 = X2 - X1;
	_T dY1 = Y2 - Y1;
	_T dX2 = X3 - X2;
	_T dY2 = Y3 - Y2;

	return dX1 * dY2 - dX2 * dY1;
}

template <class _T>
long mcGeoc<_T>::TurningOfArea(_T *LineT, long SizeT)
{
	long t = 0;

	_T *TempT = LineT;
	for(int i = 2; i < SizeT; i++, TempT += 2)
	{
		t += TurningOfLine(TempT);
	}
	{
		t += TurningOfLine(TempT[0], TempT[1], TempT[2], TempT[3], LineT[0], LineT[1]);
		t += TurningOfLine(TempT[2], TempT[3], LineT[0], LineT[1], LineT[2], LineT[3]);
	}
	return t;
}


/////////////////////////////////////////////////////////////////////////////
//
// mcGeoc end
//
/////////////////////////////////////////////////////////////////////////////

}

#endif
